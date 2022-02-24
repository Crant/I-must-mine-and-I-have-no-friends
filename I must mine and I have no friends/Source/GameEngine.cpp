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
void GameEngine::PingServer()
{
	mPingTimer += GetElapsedTime();

	if (mPingTimer >= mPingDelay)
	{
		mClient->PingServer();
		mPingTimer = 0.0f;
	}
}

void GameEngine::Init(const std::string& seedName, int worldWidth, int worldHeight, float fGravity)
{
	mMapGen->GenerateMap(seedName, worldWidth, worldHeight, MapType::EarthlikePlanet, fGravity);
	//nBlockSeeds = Maths::GetWorldBlockSeeds(seedName, worldWidth, worldHeight); //Se alltid till att blockseeds lagras efter v�rlden �r skapad

	Tiles::LoadTiles();

	GlobalState::Init(olc::vf2d(0, 0), ScreenWidth(), ScreenHeight(), this);

	mTempPlayer = std::make_shared<Player>(olc::vf2d(worldWidth / 2, worldHeight / 2), olc::vf2d(2.f, 3.f), SpriteType::Player, 10.f);
	auto pShooiter = std::make_shared<PhysObjShooter>("PhysGun", ItemType::Default);
	auto pminelaser = std::make_shared<MiningLaser>("MiningLaser", ItemType::MiningLaser);
	mTempPlayer->GetInventory()->AddItemToInventory(pShooiter);
	mTempPlayer->GetInventory()->AddItemToInventory(pminelaser);
	EntityManager::AddObj(mTempPlayer);
	//GlobalState::Init(mTempPlayer->vPos, ScreenWidth(), ScreenHeight(), this);
	//mTestMenu = std::make_unique<DynamicMenu>(olc::vf2d(300, 150), mTempPlayer->GetInventory());

	mGameState = GameState::GameLoopState;
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
	Clear(olc::BLACK);

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

	//mPerf->PreMeasure("Handle Net Messages", 2);
	HandleNetworkMessages();
	//mPerf->PostMeasure("Handle Net Messages", 2);
	//renderer.UpdateCamera();
		//mPerf->PreMeasure("Render", 0);
	GFX::RenderTiles();
	//mPerf->PostMeasure("Render", 0);
	EntityManager::UpdatePhysObjs();
	GlobalState::Update(mTempPlayer->vPos); //M�ste uppdatera efter allt allt annat
	mTempPlayer->UpdateUI(); //Samma h�r f�r att det inte ska rendera under saker
	//EntityManager::UpdateEvents();

	mPerf->PreMeasure("Update GameObjects", 0);
	UpdateGameObjects();
	mPerf->PostMeasure("Update GameObjects", 0);

	OnUserFixedUpdate();
	if (mTempChest == nullptr && GetKey(olc::Key::K).bPressed)
	{
		mTempChest = std::make_shared<Storage>(olc::vi2d(GlobalState::GetMouseWorld().x, GlobalState::GetMouseWorld().y), olc::vi2d(3, 3), SpriteType::Storage, 9);
		EntityManager::AddObj(mTempChest);
		auto pShooiter = std::make_shared<PhysObjShooter>("PhysGun", ItemType::Default);
		mTempChest->cInventory->AddItemToInventory(pShooiter);
	}
	else if (mTempChest2 == nullptr && GetKey(olc::Key::K).bPressed)
	{
		mTempChest2 = std::make_shared<Storage>(olc::vi2d(GlobalState::GetMouseWorld().x, GlobalState::GetMouseWorld().y), olc::vi2d(3, 3), SpriteType::Storage, 9);
		EntityManager::AddObj(mTempChest2);
		auto pShooiter = std::make_shared<PhysObjShooter>("PhysGun", ItemType::Default);
		mTempChest2->cInventory->AddItemToInventory(pShooiter);
	}


	//mTestMenu->Update(this);
	//mTestMenu->Render(this);

	if (GetKey(olc::Key::ENTER).bHeld)
		return false;

	return true;
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
		//if (GetMouse(0).bHeld)
		//{
		//	//cObjects.insert(std::make_pair(GlobalState::GetMouseWorld(), std::make_shared<PhysObj>(GlobalState::GetMouseWorld(), olc::vf2d(1.f, 1.f), SpriteType::Default)));
		//	//EntityManager::AddObj(std::make_shared<PhysObj>(GlobalState::GetMouseWorld(), olc::vf2d(1.f, 1.f), SpriteType::Default));
		//	World::Main()->DamageBlock(GlobalState::GetMouseWorld(), 100.0f);
		//}
		mWorld->CheckDamagedTiles();
		
		if (mIsServer)
			SendUnitUpdates();

		//physX.UpdatePhysics(this);
		fTimer = 0.0f;
	}
}
