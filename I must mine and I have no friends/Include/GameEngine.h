#pragma once

#include "olcPixelGameEngine.h"
#include "World.h"
#include "GridGenerator.h"
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

	class Player
	{
	public:
		Player()
		{
			mPos = olc::vf2d(.0f, .0f);
			mVel = olc::vf2d(50.0f, 50.0f);
		}
		olc::vf2d mPos;
		olc::vf2d mVel;
	};
	//TEMP
	struct MousePositionWorld
	{
		float x = 0;
		float y = 0;
	};
	//TEMP
	struct CameraPositionWorld
	{
		float x = 0;
		float y = 0;
	};
	//TEMP
	struct VisibleTiles
	{
		int x = 0;
		int y = 0;
	};

	class GameEngine : public olc::PixelGameEngine, public Observer
	{
	public:
		GameEngine();

		virtual ~GameEngine();

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

		//GFX
		void Render();
		void InitCameraSettings();
		void CheckMovement();
		void RandomInputs();

	protected:
		virtual void OnEvent(Event* e);

	private:
		static inline GameEngine* Instance;

		std::shared_ptr<World> mWorld;

		GameState mGameState;
		//Renderer renderer;
		TilePhysics physX;
		TileController tileControl;

		uint32_t worldWidth = 500;
		uint32_t worldHeight = 500;

		const float fFixedUpdate = 0.02f;
		float fTimer;

		olc::Sprite* sprSht;
		olc::Decal* decSht;

		//TEMP
		std::unique_ptr<Player> mTempPlayer;
		CameraPositionWorld mCamera;
		MousePositionWorld mMousePos;
		VisibleTiles mVisibleTiles;

		int mTileSize = 8;
		int mPixelSize = 32;
		float mTileScale = 0.0f;
		float mOffsetX;
		float mOffsetY;
		//Network
		bool mIsServer;

		std::unique_ptr<Client> mClient;
		std::unique_ptr<Server> mServer;

		float mPingDelay;
		float mPingTimer;
	};
}