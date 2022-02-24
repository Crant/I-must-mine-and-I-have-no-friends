#pragma once

//#include "olcPixelGameEngine.h"
#include "Button.h"
#include "Observer.h"
#include "Inventory.h"
namespace IMM
{
	class Menu : public Observed
	{
	public:
		//Menu(const olc::vf2d& vStartPos); //f�r dynamisk
		Menu(const olc::vf2d& vStartPos, const olc::vf2d& vSize);
		Menu(const olc::vf2d& vStartPos);
		virtual void Update(olc::PixelGameEngine* pge);
		virtual void Render(olc::PixelGameEngine* pge);

		std::map<int, std::unique_ptr<Button>> mButtons;
		olc::vf2d vPos;
		olc::vf2d vEndPos;
		olc::vf2d vSize;
		olc::vi2d vMid;

	protected:

	};
	class DynamicMenu
	{
	public:
		DynamicMenu(const olc::vf2d& vStartPos, std::shared_ptr<Inventory> cInventory);
		void Update(olc::PixelGameEngine* pge);
		void Render(olc::PixelGameEngine* pge);
		bool bMenuActive = false;

		std::map<int, std::unique_ptr<Button>> mButtons;

		olc::vf2d vPos;
		olc::vf2d vEndPos;
		olc::vf2d vStoragePos;
		//olc::vf2d vSize;
		//olc::vi2d vMid;

		olc::vf2d vPanelSize;
		olc::vf2d vPanelOffset;
		olc::vf2d vMenuSize;
		olc::vi2d vMouseToInv;

		olc::vf2d vStartDragPos;
		olc::vf2d vDragOffset;

		int nMenuPanels;
		int nItemPressed;


		std::shared_ptr<Inventory> cInventory;


		bool bItemHeld = false;

	protected:

	};
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

	class DynamicInventoryMenu : public Observed
	{
	public:
		DynamicInventoryMenu(std::shared_ptr<Inventory> mInventory,  olc::PixelGameEngine* pge);
		~DynamicInventoryMenu();

	private:

	};


}
