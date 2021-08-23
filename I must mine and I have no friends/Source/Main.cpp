#define OLC_PGE_APPLICATION
#include "../Include/olcPixelGameEngine.h"
#include "../Include/World.h"
#include "../Include/GridGenerator.h"
#include "../Renderer.h"

using namespace IMM;

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
			GridGenerator gridGen("Bruh", worldWidth, worldHeight);
		    World::GetWorld()->SetWorld(worldWidth, worldHeight, gridGen.GenerateWorld(), "Bruh");
			
			renderer.SetCamera(this);
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			// called once per frame
			renderer.MoveCamera(this);
			return true;
		}
	private:
		Renderer renderer;
		int worldWidth = 500;
		int worldHeight = 500;
	};
}
	int main()
	{
		Game game;

		if (game.Construct(640, 480, 2, 2))
			game.Start();

		return 0;
	}
