#pragma once
#include "Item.h"
#include "WorldEvents.h"
#include <set>

using namespace IMM;

namespace IMM
{
	class Inventory
	{
	public:
		Inventory(int nStorageAmount);
		Inventory();
		//std::set<ItemType> sItems;
		//std::set<TileType> sTiles;
		std::vector<std::shared_ptr<Item>> mStorage;

		bool AddItemToInventory(std::shared_ptr<Item> eItem); //Default för att ge item
		bool AddItemToInventoryAtPos(std::shared_ptr<Item> eItem, int nNewPos, int nOldPos);
		bool HasItemInInventory(ItemType eItem);
		bool HasBlockInInventory(TileType eTile);
		bool RemoveItemFromInventory(std::shared_ptr<Item> eItem);
	private:
	};
}



