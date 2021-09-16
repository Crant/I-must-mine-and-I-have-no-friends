#pragma once

#include "olcPixelGameEngine.h"
#include "Button.h"
#include "Observer.h"
namespace IMM
{
	class MainMenu : public Observed
	{
	public:
		MainMenu(int screenWidth, int screenHeight);
		void Update(olc::PixelGameEngine* pge);
		void Render(olc::PixelGameEngine* pge);

	private:
		std::unique_ptr<Button> mHostGameButton;
		std::unique_ptr<Button> mJoinGameButton;
	};
}
