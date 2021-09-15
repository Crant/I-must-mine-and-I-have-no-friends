#include "Client.h"

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

void Client::RequestWorldInfo()
{
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::ClientRequestWorldInfo;
	Send(msg);
}

void Client::RequestWorldData()
{
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::ClientRequestWorldData;
	Send(msg);
}
void Client::RequestTileChange(float tileX, float tileY, IMM::TileType tt)
{
	IMM::Network::Message<NetworkMessageTypes> msg;
	msg.mHeader.mID = NetworkMessageTypes::ClientChangeTileRequest;

	msg << tt << tileX << tileY;

	Send(msg);
}