#pragma once

#include "World.h"
#include "olcPixelGameEngine.h"

namespace IMM
{
	class TilePhysics
	{
	public:
		void UpdatePhysics(olc::PixelGameEngine* pge);
	private:
		float fTotalTime = 0.0f;
		void UpdateNbours();
	};


}


