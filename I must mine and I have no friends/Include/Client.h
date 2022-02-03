#pragma once

#include "NetworkMessages.h"
#include "Tiles.h"

namespace IMM
{
	static class GFXGlobals
	{
	public:
		GFXGlobals();
		~GFXGlobals();

		void Init(int nTileSize, olc::vf2d vCameraInit);

	private:

	};

	class Client : public IMM::Network::ClientInterface<NetworkMessageTypes>
	{
	private:
		IMM::Network::TsQueue<IMM::Network::OwnedMessage<NetworkMessageTypes>> mOnHoldQ;

	public:
		void PingServer();

		void MessageAll();
		void RequestWorldInfo();
		void RequestWorldData();
		void RequestTileChange(float tileX, float tileY, IMM::TileType tt);
		void PutMsgOnHold(IMM::Network::OwnedMessage<NetworkMessageTypes> msg) { mOnHoldQ.push_back(msg); }

		IMM::Network::TsQueue<IMM::Network::OwnedMessage<NetworkMessageTypes>>& GetOnHoldMsgs() { return mOnHoldQ; }
	};
}
