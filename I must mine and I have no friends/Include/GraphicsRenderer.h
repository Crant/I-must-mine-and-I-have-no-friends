#pragma once
//#include "olcPixelGameEngine.h"
#include "Entity.h"
#include "World.h"

using namespace IMM;

namespace IMM
{
	namespace GFX
	{

		void RenderBackground();
		void RenderTiles();
		void RenderSprites(IMM::Entity* sprite);
		void RenderForeground();

		int CheckNeighbour(int x, int y);
		void TileDebugger();
		void StringDebugger(const std::string& sDebug);
		void StringSpam();
	}
}





