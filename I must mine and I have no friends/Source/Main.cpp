#define OLC_PGE_APPLICATION
#include "../Include/olcPixelGameEngine.h"
#include "../Include/World.h"
#include "../Include/GridGenerator.h"
#include "../Renderer.h"
#include "../TilePhysics.h"
#include "../TileController.h"

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
			
			Assets::get().LoadSprites();
			GridGenerator gridGen("Bruh", worldWidth, worldHeight, this);
			Tiles::LoadTiles();
		    //World::Main()->SetWorld(worldWidth, worldHeight, gridGen.GenerateWorld(), "Bruh");
			renderer.SetCamera();
			return true;
		}
		bool OnUserUpdate(float fElapsedTime) override
		{
			// called once per frame
			Clear(olc::DARK_BLUE);

			renderer.UpdateCamera();
			OnUserFixedUpdate();

			if (GetKey(olc::Key::ENTER).bHeld)
				return false;

			return true;
		}
		void OnUserFixedUpdate() 
		{
			fTimer += GetElapsedTime();
			if (fTimer > fFixedUpdate)
			{
				TileController::CheckActiveTiles();
				//physX.UpdatePhysics(this);
				fTimer = 0.0f;
			}
		}
	private:
		Renderer renderer;
		TilePhysics physX;
		TileController tileControl;

		int worldWidth = 500;
		int worldHeight = 500;

		const float fFixedUpdate = 0.02f;
		float fTimer;

		olc::Sprite* sprSht;
		olc::Decal* decSht;
	};
}
	int main()
	{
		Game game;

		if (game.Construct(640, 480, 2, 2, false))
			game.Start();

		return 0;
	}
