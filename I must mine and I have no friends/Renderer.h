#pragma once
#include "../I must mine and I have no friends/Include/olcPixelGameEngine.h"
#include "Include/World.h"
#include "AssetsManager.h"

namespace IMM 
{
	class Renderer
	{

	public:
		Renderer(){}

		void UpdateCamera();
		void SetCamera(olc::PixelGameEngine* pge);
	private:
		float fCameraPosX = 50.0f;
		float fCameraPosY = 50.0f;

		float fMousePosX;
		float fMousePosY;

		int nVisibleTilesX;
		int nVisibleTilesY;

		float fOffsetX;
		float fOffsetY;
		/*
		olc::vf2d pos = olc::vf2d(x * tileSize - fTileOffsetX, y * tileSize - fTileOffsetY); Position att rendera

		x + fOffsetX, y + fOffsetY F�r att f� ut vilken vilken tile som p�verkas
		
		
		
		
		
		
		*/
		int tileSize = 16;
		int pixelSize = 32;
		float tileScale = 0.0f;

		olc::PixelGameEngine* pge;
		olc::Sprite* sDirt;
		olc::Decal* dDirt;

		void MoveCamera();
		void RenderCamera();
	};

}
