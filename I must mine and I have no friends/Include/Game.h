#include "olcPixelGameEngine.h"
#include "../Include/World.h"
#include "../Include/GridGenerator.h"
#include "../Renderer.h"
#include "../TilePhysics.h"
#include "../TileController.h"
#include "../PerlinNoise.h"

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

		static Game* Main()
		{
			if (Instance == nullptr)
				Instance = new Game();

			return Instance;
		}

	public:

		bool OnUserCreate() override;
		bool OnUserUpdate(float fElapsedTime) override;
		void OnUserFixedUpdate();

	private:
		static inline Game* Instance;

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