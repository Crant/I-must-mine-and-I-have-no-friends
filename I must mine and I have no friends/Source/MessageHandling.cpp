#include "GameEngine.h"
#include "NetworkMessages.h"
#include "WorldEvents.h"
#include "safe.h"
#include "AssetsManager.h"
#include "MenuEvents.h"
#include "Globals.h"
#include "EntityManager.h"
#include "GraphicsRenderer.h"

using namespace IMM;

void GameEngine::HandleNetworkMessage(IMM::Network::Message<NetworkMessageTypes>& msg)
{
	switch (msg.mHeader.mID)
	{
	case NetworkMessageTypes::ServerSendTileChange:
	{
		olc::vi2d tilePos;
		IMM::TileType tileType;

		msg >> tileType >> tilePos;

		mWorld->SetTile(tilePos, tileType);
	}
	break;
	case NetworkMessageTypes::MessageAll:

		break;
	case NetworkMessageTypes::ServerPing:
	{
		// Server has responded to a ping request
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point timeThen;
		msg >> timeThen;
		std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
	}
	break;
	case NetworkMessageTypes::ServerSendWorldChanges:
	{
		mGameState = GameState::GameLoopState;
	}
	break;
	//case NetworkMessageTypes::ServerSendWorldSeed:
	//{
	//	std::string seedName = "";
	//	int width = 0;
	//	int height = 0;
	//	int size = 0;
	//	msg >> size;
	//
	//	for (int i = 0; i < size; i++)
	//	{
	//		char value;
	//		msg >> value;
	//
	//		seedName.push_back(value);
	//	}
	//
	//	msg >> height >> width;
	//
	//	Init(seedName, width, height);
	//}	
	//break;
	case NetworkMessageTypes::ServerSendWorldFull:
	{
		mPerf->PreMeasure("Init world from server", 3);
		int width = 0;
		int height = 0;

		msg >> height >> width;

		Tile* world = new Tile[width * height];
		for (int i = 0; i < width * height; i++)
		{
			msg >> world[i];
		}

		mWorld = std::make_shared<World>(width, height, world);

		mPerf->PostMeasure("Init world from server", 3);
		//renderer.SetWorld(mWorld);

		Tiles::LoadTiles();
		//renderer.SetCamera();
		mGameState = GameState::GameLoopState;
	}
	break;
	case NetworkMessageTypes::ServerAccept:
	{
		// Server has Accepted the connection			
		std::cout << "Server Accepted Connection\n";
		if (!mIsServer)
			mClient->RequestWorldData();
	}
	break;
	default:
		break;
	}
}

void GameEngine::HandleNetworkMessages()
{
	//Check if there are any messages to read.
	while (!mClient->Incoming().empty())
	{
		IMM::Network::OwnedMessage<NetworkMessageTypes> aMsg = mClient->Incoming().pop_front();
		IMM::Network::Message<NetworkMessageTypes> msg = aMsg.msg;

		HandleNetworkMessage(msg);
	}
}

void GameEngine::ReadOnHoldMessages()
{
	//Check if there are any messages to read.
	while (!mClient->GetOnHoldMsgs().empty())
	{
		IMM::Network::OwnedMessage<NetworkMessageTypes> aMsg = mClient->GetOnHoldMsgs().pop_front();

		HandleNetworkMessage(aMsg.msg);
	}
}

void GameEngine::CheckForWorldInitMessage()
{
	//Check if there are any messages to read.
	while (!mClient->Incoming().empty())
	{
		IMM::Network::OwnedMessage<NetworkMessageTypes> aMsg = mClient->Incoming().pop_front();
		IMM::Network::Message<NetworkMessageTypes> msg = aMsg.msg;

		switch (msg.mHeader.mID)
		{
		case NetworkMessageTypes::MessageAll:

			break;
		case NetworkMessageTypes::ServerPing:
		{
			// Server has responded to a ping request
			std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
			std::chrono::system_clock::time_point timeThen;
			msg >> timeThen;
			std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
		}
		break;
		case NetworkMessageTypes::ServerSendWorldFull:
		{
			mPerf->PreMeasure("Init world from server", 0);
			int width = 0;
			int height = 0;

			msg >> height >> width;

			Tile* world = new Tile[width * height];
			for (int i = 0; i < width * height; i++)
			{
				msg >> world[i];
			}

			mWorld = std::make_shared<World>(width, height, world);
			mPerf->PostMeasure("Init world from server", 0);
			//renderer.SetWorld(mWorld);

			Tiles::LoadTiles();
			//renderer.SetCamera();
			mGameState = GameState::GameLoopState;
		}
		break;
		default:
			mClient->PutMsgOnHold(aMsg);
			break;
		}
	}
}

