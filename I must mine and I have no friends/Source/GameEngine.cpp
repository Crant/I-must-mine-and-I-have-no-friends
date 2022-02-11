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

GameEngine::GameEngine()
{
	mIsServer = false;

	mGameState = GameState::MainMenuState;

	mPingDelay = 1.0f;
	mPingTimer = 0.0f;

	sAppName = "I must mine and i have no friends";
}

GameEngine::~GameEngine()
{
	mPerf->GenerateReport();
	//IMM::Utils::SAFE_DELETE(Instance);
}

bool GameEngine::IsServer()
{
	return mIsServer;
}

void GameEngine::Init(const std::string& seedName, int worldWidth, int worldHeight, float fGravity)
{
	nPerlinBlocks = mMapGen->GenerateMap(seedName, worldWidth, worldHeight, MapType::EarthlikePlanet, fGravity);
	nBlockSeeds = Maths::GetWorldBlockSeeds(seedName, worldWidth, worldHeight); //Se alltid till att blockseeds lagras efter världen är skapad

	Tiles::LoadTiles();

	mTempPlayer = std::make_shared<Player>(olc::vf2d(worldWidth / 2, worldHeight / 2), olc::vf2d(2.f, 3.f), SpriteType::Player, 10.f);
	auto pShooiter = std::make_shared<PhysObjShooter>("PhysGun", ItemType::Default);
	auto pminelaser = std::make_shared<MiningLaser>("MiningLaser", ItemType::MiningLaser);
	mTempPlayer->GetInventory()->AddItemToInventory(pShooiter);
	mTempPlayer->GetInventory()->AddItemToInventory(pminelaser);
	EntityManager::AddObj(mTempPlayer);
	GlobalState::Init(mTempPlayer->vPos, ScreenWidth(), ScreenHeight(), this);

	mGameState = GameState::GameLoopState;
}

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

void GameEngine::OnEvent(Event* e)
{
	if (IMM::Events::WorldCreatedEvent* WCE = dynamic_cast<IMM::Events::WorldCreatedEvent*>(e))
	{
		mWorld = WCE->world;

		//renderer.SetWorld(mWorld);
		mServer->SetWorld(mWorld);

		mWorld->AddObserver(mServer.get());
	}
	else if (IMM::Events::HostButtonPressedEvent* HBPE = dynamic_cast<IMM::Events::HostButtonPressedEvent*>(e))
	{
		mIsServer = true;

		mServer = std::make_unique<Server>(61001);
		mServer->Start();

		mClient = std::make_unique<Client>();
		mClient->Connect("127.0.0.1", 61001);

		mGameState = GameState::InitWorldState;
		mPerf->SetFilePath("Perf_Server.txt");
		Init("OOOhyer", worldWidth, worldHeight, 60.f);


	}
	else if (IMM::Events::JoinButtonPressedEvent* JBPE = dynamic_cast<IMM::Events::JoinButtonPressedEvent*>(e))
	{
		mClient = std::make_unique<Client>();
		mClient->Connect("127.0.0.1", 61001);
		mPerf->SetFilePath("Perf_Client.txt");
		mGameState = GameState::InitWorldState;
	}
}

bool GameEngine::OnUserCreate()
{
	// Called once at the start, so create things here

	mPerf = std::make_unique<IMM::Utils::PerformanceTest>();
	
	//cObjects.insert(std::make_pair(mTempPlayer->vPos, mTempPlayer));
	//cObjects.push_back(mTempPlayer);
	//World::Main()->SetWorld(worldWidth, worldHeight, gridGen.GenerateWorld(), "Bruh");
	Assets::Main()->LoadSprites();
	//renderer.SetGameEngine();
	mMainMenu = std::make_unique<MainMenu>(ScreenWidth(), ScreenHeight());
	mMapGen = std::make_shared<MapGenerator>();
	mMapGen->AddObserver(this);
	mMainMenu->AddObserver(this);
	return true;
}

bool GameEngine::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::DARK_BLUE);

	switch (mGameState)
	{
	case GameState::GameLoopState:
		return GameLoop();
		break;
	case GameState::MainMenuState:
		//Render Menu
		//GetMainMenuInput(); // TEMP server / client choice
		mMainMenu->Update(this);
		mMainMenu->Render(this);
		break;
	case GameState::InitWorldState:
		PingServer();
		HandleNetworkMessages();
		break;
	default:
		break;
	}

	return true;
}

bool GameEngine::GameLoop()
{
	if (mIsServer)
		mServer->Update(-1, false);

	//Check if we are still connected.
	if (!mClient->IsConnected())
		return false;

	PingServer();

	mPerf->PreMeasure("Handle Net Messages", 2);
	HandleNetworkMessages();
	mPerf->PostMeasure("Handle Net Messages", 2);
	//renderer.UpdateCamera();
	EntityManager::UpdatePhysObjs();

	OnUserFixedUpdate();
	
	mPerf->PreMeasure("Render", 0);
	GFX::RenderTiles();
	mPerf->PostMeasure("Render", 0);

	if (GetKey(olc::Key::ENTER).bHeld)
		return false;

	return true;
}

void GameEngine::PingServer()
{
	mPingTimer += GetElapsedTime();

	if (mPingTimer >= mPingDelay)
	{
		mClient->PingServer();
		mPingTimer = 0.0f;
	}
}

void GameEngine::OnUserFixedUpdate()
{
	fTimer += GetElapsedTime();
	if (fTimer > fFixedUpdate)
	{
		//if (GetMouse(0).bHeld)
		//{
		//	//cObjects.insert(std::make_pair(GlobalState::GetMouseWorld(), std::make_shared<PhysObj>(GlobalState::GetMouseWorld(), olc::vf2d(1.f, 1.f), SpriteType::Default)));
		//	//EntityManager::AddObj(std::make_shared<PhysObj>(GlobalState::GetMouseWorld(), olc::vf2d(1.f, 1.f), SpriteType::Default));
		//	World::Main()->DamageBlock(GlobalState::GetMouseWorld(), 100.0f);
		//}
		mWorld->CheckDamagedTiles();
		//physX.UpdatePhysics(this);
		fTimer = 0.0f;
	}
}
