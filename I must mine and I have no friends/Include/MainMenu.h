#pragma once

#include "olcPixelGameEngine.h"
#include "Button.h"
#include "Observer.h"
class MainMenu : public Observed
{
public:
	MainMenu(int screenWidth, int screenHeight);
	void Update(olc::PixelGameEngine* pge);
	void Render(olc::PixelGameEngine* pge);

protected:
	virtual void OnEvent(Event* e);

private:
	std::unique_ptr<Button> mHostGameButton;
	std::unique_ptr<Button> mJoinGameButton;
};