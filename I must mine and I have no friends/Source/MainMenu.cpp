#include "MainMenu.h"
#include "MenuEvents.h"

#define DESIGN_SCREEN_WIDTH 1920.0f
#define DESIGN_SCREEN_HEIGHT 1080.0f

IMM::MainMenu::MainMenu(int screenWidth, int screenHeight)
{
	float screenWidthScale = (float)screenWidth / DESIGN_SCREEN_WIDTH;
	float screenHeightScale = (float)screenHeight / DESIGN_SCREEN_HEIGHT;

	olc::vf2d startPosition = olc::vf2d(
		810 * screenWidthScale,
		200 * screenHeightScale);

	mHostGameButton = std::make_unique<Button>(
		"Assets/hostgameimg.png", 
		startPosition, 
		olc::vf2d(screenWidthScale, screenHeightScale));

	startPosition += olc::vi2d(0, mHostGameButton->GetScaledHeight() * 1.1f);

	mJoinGameButton = std::make_unique<Button>(
		"Assets/joingameimg.png",
		startPosition,
		olc::vf2d(screenWidthScale, screenHeightScale));
}

void IMM::MainMenu::Update(olc::PixelGameEngine* pge)
{
	//Hovered

	if (mHostGameButton->Hovered(pge->GetMouseX(), pge->GetMouseY()))
	{
		if (pge->GetMouse(0).bPressed)
		{
			//Send an Event

			IMM::Events::HostButtonPressedEvent HBPE;
			NotifyObservers(&HBPE);
		}
	}
	if (mJoinGameButton->Hovered(pge->GetMouseX(), pge->GetMouseY()))
	{
		if (pge->GetMouse(0).bPressed)
		{
			//Send an Event
			IMM::Events::JoinButtonPressedEvent JBPE;
			NotifyObservers(&JBPE);
		}
	}

	//Pressed

	//if (mHostGameButton->Pressed(pge->GetMouseX(), pge->GetMouseY()))
	//{
	//	//Send an Event
	//}
	//if (mJoinGameButton->Pressed(pge->GetMouseX(), pge->GetMouseY()))
	//{
	//	//Send an Event
	//}
}

void IMM::MainMenu::Render(olc::PixelGameEngine* pge)
{
	mHostGameButton->Render(pge);
	mJoinGameButton->Render(pge);
}