#pragma once

#include "olcPixelGameEngine.h"
#include "World.h"
#include "GridGenerator.h"
#include "TilePhysics.h"
#include "Server.h"
#include "Client.h"
#include "PerlinNoise.h"
#include "Observer.h"
#include "PerformanceTest.h"
#include "MainMenu.h"
#include "Player.h"

namespace IMM
{
	enum class GameState
	{
		MainMenuState,
		GameLoopState,
		InitWorldState,
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
		/*! Initialize the world and other variables*/
		void Init(const std::string& seedName, int worldWidth, int worldHeight);
		bool GameLoop();

		void UpdateGameObjects();

		//////////////////////
		//					//
		//	   Network		//
		//			  		//
		//////////////////////
		/*! Read every Network Message*/
		void HandleNetworkMessages();
		/*! Read the network messages and waits for world data and puts the rest on hold*/
		void CheckForWorldInitMessage();
		/*! Reads messages that were put on hold*/
		void ReadOnHoldMessages();
		/*! Handles a network message*/
		void HandleNetworkMessage(IMM::Network::Message<NetworkMessageTypes>& msg);
		/*! Ping the server*/
		void PingServer();
		/*! Send Updates for unit positions and stuff to the clients*/
		void SendUnitUpdates();

		//////////////////////
		//					//
		//	   GFX			//
		//			  		//
		//////////////////////
		void Render();
		void InitCameraSettings();
		void CheckMovement();
		void RandomInputs();

	protected:
		virtual void OnEvent(Event* e);

	private:
		std::unique_ptr<IMM::Utils::PerformanceTest> mPerf;

		std::shared_ptr<World> mWorld;

		GameState mGameState;
		//Renderer renderer;
		TilePhysics physX;

		uint32_t worldWidth = 500;
		uint32_t worldHeight = 500;

		const float fFixedUpdate = 0.02f;
		float fTimer;

		olc::Sprite* sprSht;
		olc::Decal* decSht;

		//maps an Server_ID to a Player
		std::unordered_map<uint32_t, std::unique_ptr<Player>> mPlayers;
		uint32_t mLocalPlayerID;
		//TEMP
		CameraPositionWorld mCamera;
		MousePositionWorld mMousePos;
		VisibleTiles mVisibleTiles;
		
		//Menu
		std::unique_ptr<MainMenu> mMainMenu;

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