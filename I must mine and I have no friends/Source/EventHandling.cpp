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

void GameEngine::OnEvent(Event* e) // VI MÅSTE HA EN FUNKTION DÄR ALLA OBSERVERS LÄGGS TILL
{
	if (IMM::Events::WorldCreatedEvent* WCE = dynamic_cast<IMM::Events::WorldCreatedEvent*>(e))
	{
		mWorld = WCE->world;

		//renderer.SetWorld(mWorld);
		mServer->SetWorld(mWorld);

		mWorld->AddObserver(mServer.get());
		//mWorld->AddObserver(&EntityManager::events); 
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
		Init("OHYEER", worldWidth, worldHeight, 60.f);


	}
	else if (IMM::Events::JoinButtonPressedEvent* JBPE = dynamic_cast<IMM::Events::JoinButtonPressedEvent*>(e))
	{
		mClient = std::make_unique<Client>();
		mClient->Connect("127.0.0.1", 61001);
		mPerf->SetFilePath("Perf_Client.txt");
		mGameState = GameState::InitWorldState;
	}
}