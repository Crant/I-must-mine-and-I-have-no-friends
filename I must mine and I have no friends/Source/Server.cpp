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
		case NetworkMessageTypes::ClientRequestWorld:
		{
			IMM::Network::Message<NetworkMessageTypes> msg;
			msg.mHeader.mID = NetworkMessageTypes::ServerSendWorldSeed;

			//IMM::Network::Message<NetworkMessageTypes> msg;
			//msg.mHeader.mID = NetworkMessageTypes::ServerSendWorldWidth;

			//uint32_t w = 250;
			//msg << w;

			std::string seedName = World::Main()->GetName();

			WorldInfo wi;
			wi.height = World::Main()->GetHeight();
			wi.width = World::Main()->GetWidth();

			//wi.seed = World::Main()->GetSeed();
			for (int i = 0; i < seedName.size(); i++)
			{
				wi.seed[i] = seedName[i];
			}
			msg << wi;
			//msg << wi.height;
			//msg << wi.width;
			
			MessageClient(client, msg);
			std::cout << "Sending data: " << msg << std::endl;
			
		}
	}
}