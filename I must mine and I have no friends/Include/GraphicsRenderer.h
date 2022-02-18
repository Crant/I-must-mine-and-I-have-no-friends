#pragma once
//#include "olcPixelGameEngine.h"
//#include "Entity.h"
#include "World.h"
#include "MainMenu.h"
//#include "Inventory.h"

using namespace IMM;

namespace IMM
{
	namespace GFX
	{

		void RenderBackground();
		void RenderTiles();
		void RenderSprites(IMM::Entity* sprite);
		void RenderForeground();
		void RenderInventoryMenu(std::shared_ptr<DynamicMenu> cInvMenu);

		int CheckNeighbour(int x, int y);
		void TileDebugger();
		void StringDebugger(const std::string& sDebug);
		void StringSpam();
	}
}





