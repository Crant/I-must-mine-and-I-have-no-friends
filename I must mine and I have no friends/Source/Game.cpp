#include "Game.h"
#include "NetworkMessages.h"

Game::Game()
{
	mClient = nullptr;
	mServer = nullptr;

	mIsServer = false;

	mGameState = GameState::MainMenuState;

	mPingDelay = 1.0f;
	mPingTimer = 0.0f;

	sAppName = "I must mine and i have no friends";
}

Game::~Game()
{
	if (mClient != nullptr)
	{
		mClient->Disconnect();

		delete mClient;
	}
	if (mServer != nullptr)
	{
		mServer->Stop();
		delete mServer;
	}		
}

bool Game::IsServer()
{
	return mIsServer;
}

void Game::GetMainMenuInput()
{
	if (GetKey(olc::Key::H).bPressed)
	{
		mIsServer = true;

		mServer = new Server(61001);
		mServer->Start();

		mClient = new Client();
		mClient->Connect("127.0.0.1", 61001);

		mGameState = GameState::InitWorldState;

		Init("0123456789", 500, 500);
	}
	else if (GetKey(::olc::Key::J).bPressed)
	{
		mClient = new Client();
		mClient->Connect("127.0.0.1", 61001);

		mGameState = GameState::InitWorldState;
	}
}

void IMM::Game::Init(const std::string& seedName, int worldWidth, int worldHeight)
{
	this->worldWidth = worldWidth;
	this->worldHeight = worldHeight;

	GridGenerator gridGen = GridGenerator(seedName, worldWidth, worldHeight);
	Tiles::LoadTiles();
	renderer.SetCamera();

	mGameState = GameState::GameLoopState;
}

void IMM::Game::HandleNetworkMessages()
{
	//Check if there are any messages to read.
	while (!mClient->Incoming().empty())
	{
		IMM::Network::Message<NetworkMessageTypes> msg = mClient->Incoming().pop_front().msg;

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
			int width = 0;
			int height = 0;

			msg >> height >> width;

			Tile* world = new Tile[width * height];
			for (int i = 0; i < width * height; i++)
			{
				msg >> world[i];
			}

			World::Main()->Init(width, height, world, "");

			Tiles::LoadTiles();
			renderer.SetCamera();

			mGameState = GameState::GameLoopState;
		}
		break;
		case NetworkMessageTypes::ServerAccept:
		{
			// Server has responded to a ping request				
			std::cout << "Server Accepted Connection\n";
			if (!mIsServer)
				mClient->RequestWorldData();
		}
		break;
		default:
			break;
		}
	}
	
}

bool Game::OnUserCreate()
{
	// Called once at the start, so create things here

	
	//World::Main()->SetWorld(worldWidth, worldHeight, gridGen.GenerateWorld(), "Bruh");
	Assets::Main()->LoadSprites();
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
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
		GetMainMenuInput(); // TEMP server / client choice
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

bool Game::GameLoop()
{
	if (mIsServer)
		mServer->Update(-1, false);

	//Check if we are still connected.
	if (!mClient->IsConnected())
		return false;

	PingServer();

	HandleNetworkMessages();

	renderer.UpdateCamera();

	OnUserFixedUpdate();

	if (GetKey(olc::Key::ENTER).bHeld)
		return false;

	return true;
}

void Game::PingServer()
{
	if (!mIsServer)
	{
		mPingTimer += GetElapsedTime();

		if (mPingTimer >= mPingDelay)
		{
			mClient->PingServer();
			mPingTimer = 0.0f;
		}
	}
}

void Game::OnUserFixedUpdate()
{
	fTimer += GetElapsedTime();
	if (fTimer > fFixedUpdate)
	{
		TileController::CheckActiveTiles();
		//physX.UpdatePhysics(this);
		fTimer = 0.0f;
	}
}