#include "../Include/Server.h"
#include "../Include/Game.h"

IMM::Server::Server(uint16_t nPort) : IMM::Network::ServerInterface<NetworkMessageTypes>(nPort)
{

}

void IMM::Server::OnClientValidated(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client)
{
	// Client passed validation check, so send them a message informing
	// them they can continue to communicate
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::ServerAccept;
	client->Send(msg);
}

bool IMM::Server::OnClientConnect(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client)
{
	//Server cap or other stuff
	return true;
}

void IMM::Server::OnClientDisconnect(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client)
{
	std::cout << "Removing client [" << client->GetID() << "]\n";
}

void IMM::Server::OnMessage(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client, IMM::Network::Message<NetworkMessageTypes>& msg)
{
	switch (msg.mHeader.mID)
	{
		case NetworkMessageTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->Send(msg);
		}
		break;

		case NetworkMessageTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";

			// Construct a new message and send it to all clients
			IMM::Network::Message<NetworkMessageTypes> msg;
			msg.mHeader.mID = NetworkMessageTypes::ServerMessage;
			msg << client->GetID();
			MessageAllClients(msg, client);

		}
		break;
		case NetworkMessageTypes::ClientRequestWorldInfo:
		{
			std::cout << "[" << client->GetID() << "]: Requested World Info\n";

			IMM::Network::Message<NetworkMessageTypes> msg;
			msg.mHeader.mID = NetworkMessageTypes::ServerSendWorldSeed;

			std::string seedName = World::Main()->GetName();

			msg << World::Main()->GetWidth();
			msg << World::Main()->GetHeight();

			for (int i = seedName.size() - 1; i >= 0; i--)
			{
				msg << seedName[i];
			}
			msg << seedName.size();
			
			MessageClient(client, msg);
		}
		break;
		case NetworkMessageTypes::ClientRequestWorldData:
		{
			std::cout << "[" << client->GetID() << "]: Requested World Data\n";

			IMM::Network::Message<NetworkMessageTypes> msg;
			msg.mHeader.mID = NetworkMessageTypes::ServerSendWorldFull;

			Tile* worldData = World::Main()->GetWorld();
			int worldSize = World::Main()->GetSize();
			for (int i = worldSize; i >= 0; i--)
			{
				msg << worldData[i];
				//msg << worldData[i].type << worldData[i].Nbrs;
			}
			
			msg << World::Main()->GetWidth();
			msg << World::Main()->GetHeight();

			MessageClient(client, msg);
		}
		break;
	}
}