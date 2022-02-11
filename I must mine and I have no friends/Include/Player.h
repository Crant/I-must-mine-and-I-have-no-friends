#pragma once
//#include "olcPixelGameEngine.h"
//#include "PhysicsObject.h"
#include "Client.h"
#include "Inventory.h"

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

		void UserInput();
	protected:
		void MoveUnit() override;
		void OnEvent(Event*) override;
		void UseItem();
		std::shared_ptr<Inventory> mInventory;


	private:
		const int nPickupDistance = 10;
	};
}

