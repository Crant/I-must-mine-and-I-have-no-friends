#pragma once
#include "MainMenu.h"

class Storage : public Entity
{
public:
	Storage(const olc::vf2d& vPos, const olc::vf2d& vSize, SpriteType sModel, int nInvSize);
	std::shared_ptr<Inventory> cInventory;
	std::shared_ptr<DynamicMenu> cInventoryMenu;
};