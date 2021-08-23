#pragma once
#include "../I must mine and I have no friends/Include/olcPixelGameEngine.h"
#include "Include/World.h"

namespace IMM 
{
	class Renderer
	{

	public:
		Renderer(){}

		void MoveCamera(olc::PixelGameEngine* pge);
		void SetCamera(olc::PixelGameEngine* pge);
	private:
		float fCameraPosX = 50.0f;
		float fCameraPosY = 50.0f;

		int nVisibleTilesX;
		int nVisibleTilesY;

		float fOffsetX;
		float fOffsetY;

		int tileSize = 8;
	};

}
