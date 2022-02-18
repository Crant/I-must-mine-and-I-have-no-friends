#pragma once
//#include "olcPixelGameEngine.h"
//#include "PhysicsObject.h"
#include "Client.h"
//#include "Inventory.h"
#include "Storage.h"

#include <set>

using namespace IMM;

namespace IMM
{
	class Player : public PhysUnit, public Observer
	{
	public:
		Player(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModelfloat, float fHitpoints);
		std::shared_ptr<Client> cPlayerClient;
		std::shared_ptr<Item> cActiveItem;
		std::shared_ptr<Inventory> GetInventory();
		std::shared_ptr<DynamicMenu> cInventoryMenu;

		std::list<std::shared_ptr<Storage>> vMenus;
		void UpdateUI();


		void UserInput();
	protected:
		void MoveUnit() override;
		void OnEvent(Event*) override;
		void UseItem();
		std::shared_ptr<Inventory> mInventory;



		//TEST
		


	private:
		const int nPickupDistance = 10;
		int nItemPressedIndex = 0;
		std::shared_ptr<Inventory> cFirstInvCheck;
		std::shared_ptr<Inventory> cSecondInvCheck;

		int nLastIndex = 0;
		bool bHasReleasedItem = false;

		bool UpdateMenu(std::shared_ptr<DynamicMenu> vMenu);
		void UpdateAllMenus();
		bool IsInInventory();
	};
}

