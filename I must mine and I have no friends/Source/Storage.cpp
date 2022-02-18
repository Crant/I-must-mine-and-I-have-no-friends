#include "Storage.h"


Storage::Storage(const olc::vf2d& vPos, const olc::vf2d& vSize, SpriteType sModel, int nInvSize)
	: Entity(vPos, vSize, sModel)
{
	cInventory = std::make_shared<Inventory>(nInvSize);
	cInventoryMenu = std::make_shared<DynamicMenu>(olc::vf2d(50, 50), cInventory);
	Updated::tType = ObjectType::Storage;
}