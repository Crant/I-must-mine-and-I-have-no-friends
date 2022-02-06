#pragma once

#include "olcPixelGameEngine.h"
#include "World.h"
//#include "GridGenerator.h"
#include "TilePhysics.h"
#include "Tiles.h"
#include "Server.h"
#include "Client.h"
#include "PerlinNoise.h"
#include "Observer.h"
#include "PerformanceTest.h"
#include "MainMenu.h"
#include "../MapGenerator.h"
#include "../Collision.h"
#include "Player.h"

//OM DU VILL INKLUDERA EN HEADER H�R SOM OCKS� HAR TILLG�NG TILL GAME ENGINE S� M�STE ALLA DEFINITIONER G�RAS I CPP FILEN INTE HEADERN 

namespace IMM
{
	enum class GameState
	{
		MainMenuState,
		GameLoopState,
		InitWorldState,
	};

	//class Player
	//{
	//public:
	//	Player()
	//	{
	//		mPos = olc::vf2d(.0f, .0f);
	//		mVel = olc::vf2d(50.0f, 50.0f);
	//	}
	//	olc::vf2d mPos;
	//	olc::vf2d mVel;
	//};
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
	struct DynamicRect
	{
		olc::vf2d vPos;
		olc::vf2d vSize;
		olc::vf2d vVelocity;

		float fSpeed;
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
		static inline std::list<PhysObj*> cObjects;

	private:
		/*! Initialize the world and other variables*/
		void Init(const std::string& seedName, int worldWidth, int worldHeight);
		bool GameLoop();

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

		//////////////////////
		//					//
		//	   GFX			//
		//			  		//
		//////////////////////
		void Render();
		void CheckWrapping(int ix, int& ox);
		void CheckWrapping(float ix, float& ox);
		int CheckNeighbour(int x, int y, bool recursive);
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

		uint32_t worldWidth = 1024;
		uint32_t worldHeight = 1024;

		const float fFixedUpdate = 0.02f;
		float fTimer;

		olc::Sprite* sprSht;
		olc::Decal* decSht;

		//TEMP
		PhysObj* mTempPlayer = nullptr;
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

		//Generation
		std::shared_ptr<MapGenerator> mMapGen;
		float* nPerlinBlocks = nullptr; //Tempor�r f�r att hitta v�rdet av perlin noiset f�r vilka block som ska spawnas
		unsigned int* nBlockSeeds; //Seeds f�r varje block i v�rlden 

		//Renderer stuff
		
		void StringDebugger();

	};
}