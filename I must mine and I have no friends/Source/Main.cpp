#define OLC_PGE_APPLICATION
#include "../Include/olcPixelGameEngine.h"
#include "../Include/World.h"
#include "../Include/GridGenerator.h"

namespace IMM 
{
	
	class Game : public olc::PixelGameEngine
	{
	public:
		Game()
		{
			sAppName = "I must mine and i have no friends";
		}

	public:
		bool OnUserCreate() override
		{
			// Called once at the start, so create things here
			GridGenerator gridGen("Bruh", 100, 100);
			//TileType* tihnd = gridGen.GenerateWorld();
		    World::GetWorld()->SetWorld(100, 100, gridGen.GenerateWorld(), "Bruh");

			
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			// called once per frame
			return true;
		}
	};
}
	int main()
	{
		IMM::Game game;

		if (game.Construct(640, 480, 2, 2))
			game.Start();

		return 0;
	}
