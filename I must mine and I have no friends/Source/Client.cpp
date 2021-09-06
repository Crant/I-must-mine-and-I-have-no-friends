#include "..\Include\Client.h"

using namespace IMM;

void Client::PingServer()
{
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::ServerPing;

	// Caution with this...
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

	msg << timeNow;
	Send(msg);
}

void Client::MessageAll()
{
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::MessageAll;
	Send(msg);
}

void Client::RequestWorld()
{
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::ClientRequestWorld;
	Send(msg);
}