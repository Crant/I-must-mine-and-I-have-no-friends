#pragma once

#include "World.h"
#include "AssetsManager.h"
#include "TileController.h"

namespace IMM 
{
	class GameEngine;

	class Renderer
	{

	public:
		Renderer() 
		{ 
			this->sDirt = nullptr; 
			this->dDirt = nullptr;
		}

		void UpdateCamera();
		void SetCamera();
		void SetWorld(std::shared_ptr<World> world) { mWorld = world; }
	private:
		std::shared_ptr<World> mWorld;

		float fCameraPosX = 50.0f;
		float fCameraPosY = 50.0f;

		float fMousePosX;
		float fMousePosY;

		int nVisibleTilesX;
		int nVisibleTilesY;

		float fOffsetX;
		float fOffsetY;

		float fTimer = 0.0f;
		/*
		olc::vf2d pos = olc::vf2d(x * tileSize - fTileOffsetX, y * tileSize - fTileOffsetY); Position att rendera

		x + fOffsetX, y + fOffsetY För att få ut vilken vilken tile som påverkas
		
		
		
		
		
		
		*/
		int tileSize = 8;
		int pixelSize = 32;
		float tileScale = 0.0f;
		float fCameraSpeed = 50.0f;

		olc::Sprite* sDirt;
		olc::Decal* dDirt;
		TileController tileCont;

		void MoveCamera();
		void RenderCamera();

		std::shared_ptr<std::vector<int>> Cool = nullptr;
		std::shared_ptr<std::vector<std::vector<int>>> Cooler = nullptr;
	};

}
