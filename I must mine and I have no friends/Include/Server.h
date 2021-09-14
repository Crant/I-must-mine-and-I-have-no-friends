#pragma once

#include "NetworkMessages.h"
#include "World.h"
#include "Observer.h"

namespace IMM
{
	class Server : public IMM::Network::ServerInterface<NetworkMessageTypes>, public Observer
	{
	public:
		Server(uint16_t nPort);

		void SetWorld(std::shared_ptr<World> world) { mWorld = world; }


		virtual void OnClientValidated(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client);
	protected:
		virtual bool OnClientConnect(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client);

		// Called when a client appears to have disconnected
		virtual void OnClientDisconnect(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client);

		// Called when a message arrives
		virtual void OnMessage(std::shared_ptr<IMM::Network::Connection<NetworkMessageTypes>> client, IMM::Network::Message<NetworkMessageTypes>& msg);
	
		virtual void OnEvent(Event* e);
	private:
		std::shared_ptr<World> mWorld;
	};
}
