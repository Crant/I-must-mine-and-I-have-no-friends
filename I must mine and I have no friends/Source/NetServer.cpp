//#include <iostream>
//#include <Network.h>
//
//enum class CustomMsgTypes : uint32_t
//{
//	ServerAccept,
//	ServerDeny,
//	ServerPing,
//	MessageAll,
//	ServerMessage,
//};
//
//class CustomServer : public IMM::Network::ServerInterface<CustomMsgTypes>
//{
//public:
//	CustomServer(uint16_t nPort) : IMM::Network::ServerInterface<CustomMsgTypes>(nPort)
//	{
//
//	}
//
//protected:
//	virtual bool OnClientConnect(std::shared_ptr<IMM::Network::Connection<CustomMsgTypes>> client)
//	{
//		IMM::Network::Message<CustomMsgTypes> msg;
//		msg.mHeader.mID = CustomMsgTypes::ServerAccept;
//		client->Send(msg);
//		return true;
//	}
//
//	// Called when a client appears to have disconnected
//	virtual void OnClientDisconnect(std::shared_ptr<IMM::Network::Connection<CustomMsgTypes>> client)
//	{
//		std::cout << "Removing client [" << client->GetID() << "]\n";
//	}
//
//	// Called when a message arrives
//	virtual void OnMessage(std::shared_ptr<IMM::Network::Connection<CustomMsgTypes>> client, IMM::Network::Message<CustomMsgTypes>& msg)
//	{
//		switch (msg.mHeader.mID)
//		{
//		case CustomMsgTypes::ServerPing:
//		{
//			std::cout << "[" << client->GetID() << "]: Server Ping\n";
//
//			// Simply bounce message back to client
//			client->Send(msg);
//		}
//		break;
//
//		case CustomMsgTypes::MessageAll:
//		{
//			std::cout << "[" << client->GetID() << "]: Message All\n";
//
//			// Construct a new message and send it to all clients
//			IMM::Network::Message<CustomMsgTypes> msg;
//			msg.mHeader.mID = CustomMsgTypes::ServerMessage;
//			msg << client->GetID();
//			MessageAllClients(msg, client);
//
//		}
//		break;
//		}
//	}
//};
//
//int main()
//{
//	CustomServer server(60000);
//	server.Start();
//
//	while (1)
//	{
//		server.Update(-1, true);
//	}
//
//
//
//	return 0;
//}