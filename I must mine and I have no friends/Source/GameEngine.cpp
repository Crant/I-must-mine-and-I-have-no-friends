#include "GameEngine.h"
#include "NetworkMessages.h"
#include "WorldEvents.h"
#include "safe.h"
#include "AssetsManager.h"
#include "MenuEvents.h"
#include "PlayerEvents.h"
#include "ClientEvents.h"
#include "ServerFramePacket.h"

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

void GameEngine::Init(const std::string& seedName, int worldWidth, int worldHeight)
{
	this->worldWidth = worldWidth;
	this->worldHeight = worldHeight;

	GridGenerator gridGen = GridGenerator();

	gridGen.AddObserver(this);
	mPerf->PreMeasure("Init World", 3);
	gridGen.Init(seedName, worldWidth, worldHeight);
	mPerf->PostMeasure("Init World", 3);

	Tiles::LoadTiles();

	InitCameraSettings();
	//renderer.SetCamera();

	mGameState = GameState::GameLoopState;
}
//Client
void GameEngine::HandleNetworkMessage(IMM::Network::Message<NetworkMessageTypes>& msg)
{
	switch (msg.mHeader.mID)
	{
	case NetworkMessageTypes::ServerSendUnitUpdates:
	{
		int updates = 0;

		msg >> updates;

		for (int i = 0; i < updates; i++)
		{
			ServerFramePacket sfp;

			msg >> sfp;

			mPlayers[sfp.GetID()]->SetPosition(sfp.GetPosition());
			mPlayers[sfp.GetID()]->SetDirection(sfp.GetDirection());
		}
	}
	break;
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
		InitCameraSettings();
		mGameState = GameState::GameLoopState;
	}
	break;
	case NetworkMessageTypes::ServerAccept:
	{
		// Server has Accepted the connection			
		std::cout << "Server Accepted Connection\n";

		msg >> mLocalPlayerID;

		mPlayers[mLocalPlayerID] = std::make_unique<Player>(mLocalPlayerID, "",
			"Assets/player.png", olc::vf2d(50.0f, 50.0f), olc::vf2d(0.1f, 0.1f));

		mPlayers[mLocalPlayerID]->AddObserver(this);

		if (!mIsServer)
			mClient->RequestWorldData();
	}
	break;
	default:
		break;
	}
}
//Client
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
//Client
void GameEngine::ReadOnHoldMessages()
{
	//Check if there are any messages to read.
	while (!mClient->GetOnHoldMsgs().empty())
	{
		IMM::Network::OwnedMessage<NetworkMessageTypes> aMsg = mClient->GetOnHoldMsgs().pop_front();

		HandleNetworkMessage(aMsg.msg);
	}
}
//Client
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
			InitCameraSettings();
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
	if (IMM::Events::PlayerMoveEvent* PME = dynamic_cast<IMM::Events::PlayerMoveEvent*>(e))
	{
		mClient->SendDirectionChange(PME->direction.x, PME->direction.y);
	}//Server Only Event
	else if (IMM::Events::ClientDirectionChangeEvent* CDCE = dynamic_cast<IMM::Events::ClientDirectionChangeEvent*>(e))
	{
		if(CDCE->clientID != mLocalPlayerID)
			mPlayers[CDCE->clientID]->UpdateDirection(olc::vf2d(CDCE->direction));
	}
	else if (IMM::Events::WorldCreatedEvent* WCE = dynamic_cast<IMM::Events::WorldCreatedEvent*>(e))
	{
		mWorld = WCE->world;

		//renderer.SetWorld(mWorld);
		mServer->SetWorld(mWorld);

		mWorld->AddObserver(mServer.get());
	}//Server Only Event
	else if (IMM::Events::HostButtonPressedEvent* HBPE = dynamic_cast<IMM::Events::HostButtonPressedEvent*>(e))
	{
		mIsServer = true;

		mServer = std::make_unique<Server>(61001);
		mServer->Start();

		mServer->AddObserver(this);

		mClient = std::make_unique<Client>();
		mClient->Connect("127.0.0.1", 61001);

		mGameState = GameState::InitWorldState;
		mPerf->SetFilePath("Perf_Server.txt");
		Init("123456789", 500, 500);
	}//Client Only Event
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
	
	//World::Main()->SetWorld(worldWidth, worldHeight, gridGen.GenerateWorld(), "Bruh");
	Assets::Main()->LoadSprites();
	//renderer.SetGameEngine();
	mMainMenu = std::make_unique<MainMenu>(ScreenWidth(), ScreenHeight());
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

	CheckMovement();

	mPerf->PreMeasure("Update GameObjects", 0);
	UpdateGameObjects();
	mPerf->PostMeasure("Update GameObjects", 0);

	OnUserFixedUpdate();
	
	mPerf->PreMeasure("Render", 0);
	Render();
	mPerf->PostMeasure("Render", 0);

	if (GetKey(olc::Key::ENTER).bHeld)
		return false;

	return true;
}

void IMM::GameEngine::UpdateGameObjects()
{
	for (auto it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		it->second->Update(this, GetElapsedTime());
	}

	// Clamp camera to game boundaries
	olc::vf2d playerPos = mPlayers[mLocalPlayerID]->GetPosition();
	if (playerPos.x < 0)
		playerPos.x = 0;

	if (playerPos.y < 0)
		playerPos.y = 0;

	if (playerPos.x > mWorld->GetWidth())
		playerPos.x = mWorld->GetWidth();

	if (playerPos.y > mWorld->GetHeight())
		playerPos.y = mWorld->GetHeight();

	mPlayers[mLocalPlayerID]->SetPosition(playerPos);
	mCamera.x = mPlayers[mLocalPlayerID]->GetPosition().x;
	mCamera.y = mPlayers[mLocalPlayerID]->GetPosition().y;
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

void GameEngine::SendUnitUpdates()
{
	std::vector<ServerFramePacket> packets;
	for (auto it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		ServerFramePacket sfp = ServerFramePacket(
			it->second->GetPosition(), 
			it->second->GetDirection(),
			it->first);
		
		packets.push_back(sfp);
	}

	if (packets.size() > 0)
		mServer->SendUnitUpdates(packets);
}

void GameEngine::OnUserFixedUpdate()
{
	fTimer += GetElapsedTime();
	if (fTimer > fFixedUpdate)
	{
		mWorld->CheckDamagedTiles();
		
		if (mIsServer)
			SendUnitUpdates();

		//physX.UpdatePhysics(this);
		fTimer = 0.0f;
	}
}

void GameEngine::Render()
{
	// Calculate Top-Leftmost visible tile
	mOffsetX = mCamera.x - (float)mVisibleTiles.x / 2.0f;
	mOffsetY = mCamera.y - (float)mVisibleTiles.y / 2.0f;

	// Clamp camera to game boundaries
	
	if (mOffsetX < 0)
		mOffsetX = 0;
	else if (mOffsetX > mWorld->GetWidth() - mVisibleTiles.x)
		mOffsetX = mWorld->GetWidth() - mVisibleTiles.x;

	if (mOffsetY < 0)
		mOffsetY = 0;
	else if (mOffsetY > mWorld->GetHeight() - mVisibleTiles.y)
		mOffsetY = mWorld->GetHeight() - mVisibleTiles.y;

	// Get offsets for smooth movement
	float fTileOffsetX = (mOffsetX - (int)mOffsetX) * mTileSize;
	float fTileOffsetY = (mOffsetY - (int)mOffsetY) * mTileSize;

	//if (fOffsetX < 0) fOffsetX = 0;
	//if (fOffsetY < 0) fOffsetY = 0;
	//if (fOffsetX > World::Main()->GetWidth() - nVisibleTilesX) fOffsetX = World::Main()->GetWidth() - nVisibleTilesX;
	//if (fOffsetY > World::Main()->GetHeight() - nVisibleTilesY) fOffsetY = World::Main()->GetHeight() - nVisibleTilesY;

	for (int x = -1; x < mVisibleTiles.x + 1; x++)
	{
		for (int y = -1; y < mVisibleTiles.y + 1; y++)
		{
			if (mWorld->IsBlock(x + mOffsetX, y + mOffsetY))
			{
				TileType* tile = mWorld->GetTile(olc::vf2d(x + mOffsetX, y + mOffsetY));
				int tileNbour = (int)mWorld->GetNbour(olc::vf2d(x + mOffsetX, y + mOffsetY));
				olc::vf2d pos = olc::vf2d(x * mTileSize - fTileOffsetX, y * mTileSize - fTileOffsetY);

				DrawPartialDecal(
					pos,
					Assets::Main()->GetSpriteDecal(tile),
					olc::vi2d(0, mPixelSize * tileNbour),
					olc::vi2d(mPixelSize, mPixelSize),
					olc::vf2d(mTileScale, mTileScale));
			}
		}
	}

	for (auto it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		//if (it->first == mLocalPlayerID)
			it->second->RenderLocal(this, mOffsetX, mOffsetY);
		/*else
			it->second->Render(this);*/
	}
}

void GameEngine::InitCameraSettings()
{
	mVisibleTiles.x = ScreenWidth() / mTileSize;
	mVisibleTiles.y = ScreenHeight() / mTileSize;

	mTileScale = ((float)mTileSize / (float)mPixelSize);
}

void GameEngine::CheckMovement()
{
	//SKA TROLIGEN FLYTTAS
	mMousePos.x = ((float)GetMouseX() / mTileSize) + mOffsetX; //HUR MAN FÅR TAG I MUSPEKAREN I WORLDSPACE FRÅN SKÄRM
	mMousePos.y = ((float)GetMouseY() / mTileSize) + mOffsetY;
	if (IsFocused())
	{
		olc::vf2d dir = olc::vf2d(0.0f, 0.0f);
		if (GetKey(olc::Key::W).bHeld)
		{
			dir -= olc::vf2d(0.0f, 1.0f);
			//mTempPlayer->mPos.y -= mTempPlayer->mVel.y * GetElapsedTime();
		}
		else if (GetKey(olc::Key::S).bHeld)
		{
			dir += olc::vf2d(0.0f, 1.0f);
			//mTempPlayer->mPos.y += mTempPlayer->mVel.y * GetElapsedTime();
		}
		if (GetKey(olc::Key::A).bHeld)
		{
			dir -= olc::vf2d(1.0f, 0);
			//mTempPlayer->mPos.x -= mTempPlayer->mVel.y * GetElapsedTime();
		}
		else if (GetKey(olc::Key::D).bHeld)
		{
			dir += olc::vf2d(1.0f, 0);
			//mTempPlayer->mPos.x += mTempPlayer->mVel.y * GetElapsedTime();
		}
		
		mPlayers[mLocalPlayerID]->UpdateDirection(dir);
		RandomInputs();
	}
}

void GameEngine::RandomInputs()
{
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY()), std::to_string(mCamera.x));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(mCamera.y));

	if (GetMouse(1).bHeld && !(mWorld->IsBlock(mMousePos.x, mMousePos.y)))
	{
		mClient->RequestTileChange(mMousePos.x, mMousePos.y, TileType::Dirt);

		//mWorld->CreateBlock(olc::vf2d(mMousePos.x, mMousePos.y), TileType::Dirt);
	}
	//if (Game::Main()->GetMouse(0).bHeld && World::Main()->IsBlock(fMousePosX, fMousePosY) && fTimer > 0.2f)
	//{
	//	Cool = World::Main()->FloodFill(fMousePosX, fMousePosY, Cool);
	//	//TileController::DamageBlock(World::Main()->Coordinates(World::Main()->Index(fMousePosX, fMousePosY)), 0.5f);
	//	fTimer = 0.0f;
	//}
	if (GetMouse(0).bHeld)
	{
		//Cooler = World::Main()->GetRegions(fMousePosX, fMousePosY);
		//Cooler = World::Main()->GetRegions();
		mWorld->RemoveRegions();
	}
}