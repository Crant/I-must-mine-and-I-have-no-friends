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
//
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
//	// Called when a client appears to have disconnected
//	virtual bool OnClientConnect(std::shared_ptr<IMM::Network::Connection<CustomMsgTypes>> client)
//	{
//		return true;
//	}
//
//	// Called when a client appears to have disconnected
//	virtual void OnClientDisconnect(std::shared_ptr<IMM::Network::Connection<CustomMsgTypes>> client)
//	{
//
//	}
//
//	// Called when a message arrives
//	virtual void OnMessage(std::shared_ptr<IMM::Network::Connection<CustomMsgTypes>> client, IMM::Network::Message<CustomMsgTypes>& msg)
//	{
//
//	}
//	//void PingServer()
//	//{
//	//	IMM::Network::Message<CustomMsgTypes> msg;
//	//	msg.mHeader.mID = CustomMsgTypes::ServerPing;
//
//	//	// Caution with this...
//	//	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
//
//	//	msg << timeNow;
//	//	Send(msg);
//	//}
//
//	//void MessageAll()
//	//{
//	//	IMM::Network::Message<CustomMsgTypes> msg;
//	//	msg.mHeader.mID = CustomMsgTypes::MessageAll;
//	//	Send(msg);
//	//}
//};
//enum class CustomMsg : uint32_t
//{
//	ServerAccept,
//	ServerDeny,
//
//};
//
//int main()
//{
//	CustomServer server(60000);
//	server.Start();
//
//	while (1)
//	{
//		server.Update();
//	}
//
//	return 0;
//	//Game game;
//
//	//if (game.Construct(640, 480, 2, 2))
//	//	game.Start();
//
//	//return 0;
//}