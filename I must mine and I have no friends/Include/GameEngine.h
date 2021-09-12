#pragma once

#include "olcPixelGameEngine.h"
#include "World.h"
#include "GridGenerator.h"
#include "Renderer.h"
#include "TilePhysics.h"
#include "TileController.h"
#include "Server.h"
#include "Client.h"
#include "PerlinNoise.h"
#include "Observer.h"

namespace IMM
{
	enum class GameState
	{
		MainMenuState,
		GameLoopState,
		InitWorldState,
	};

	class GameEngine : public olc::PixelGameEngine, public Observer
	{
	public:
		GameEngine();

		virtual ~GameEngine();

		static GameEngine* Main()
		{
			if (Instance == nullptr)
				Instance = new GameEngine();

			return Instance;
		}

	public:
		bool IsServer();
		
		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
		void OnUserFixedUpdate();

	private:
		// Temp, Pick if Host (H) or client (J)
		void GetMainMenuInput();
		void Init(const std::string& seedName, int worldWidth, int worldHeight);
		bool GameLoop();
		void HandleNetworkMessages();
		void PingServer();

	protected:
		virtual void OnEvent(Event* e);

	private:
		static inline GameEngine* Instance;

		std::shared_ptr<World> mWorld;

		GameState mGameState;
		Renderer renderer;
		TilePhysics physX;
		TileController tileControl;

		uint32_t worldWidth = 500;
		uint32_t worldHeight = 500;

		const float fFixedUpdate = 0.02f;
		float fTimer;

		olc::Sprite* sprSht;
		olc::Decal* decSht;

		//Network
		bool mIsServer;

		std::unique_ptr<Client> mClient;
		std::unique_ptr<Server> mServer;

		float mPingDelay;
		float mPingTimer;
	};
}