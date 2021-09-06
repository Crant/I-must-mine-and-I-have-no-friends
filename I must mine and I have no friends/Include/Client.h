#pragma once

#include "../Include/NetworkMessages.h"
namespace IMM
{
	class Client : public IMM::Network::ClientInterface<NetworkMessageTypes>
	{
	public:
		void PingServer();

		void MessageAll();
		void RequestWorldInfo();
		void RequestWorldData();
	};
}
