#define OLC_PGE_APPLICATION
#include "../Include/olcPixelGameEngine.h"

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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		return true;
	}
};

int main()
{
	Game game;

	if (game.Construct(640, 480, 2, 2))
		game.Start();

	return 0;
}