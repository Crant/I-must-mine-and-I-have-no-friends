#pragma once
#include "Include/World.h"
#include "Include/olcPixelGameEngine.h"
using namespace IMM;

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


