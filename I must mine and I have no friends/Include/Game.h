#pragma once

#include "olcPixelGameEngine.h"
#include "../Include/World.h"
#include "../Include/GridGenerator.h"
#include "../Renderer.h"
#include "../TilePhysics.h"
#include "../TileController.h"
#include "Client.h"
#include "Server.h"

namespace IMM
{
	enum class GameState
	{
		MainMenuState,
		GameLoopState,
		InitWorldState,
	};

	class Game : public olc::PixelGameEngine
	{
	public:
		Game();

		virtual ~Game();

		static Game* Main()
		{
			if (Instance == nullptr)
				Instance = new Game();

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
	private:
		static inline Game* Instance;

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

		Client* mClient;
		Server* mServer;

		float mPingDelay;
		float mPingTimer;
	};
}