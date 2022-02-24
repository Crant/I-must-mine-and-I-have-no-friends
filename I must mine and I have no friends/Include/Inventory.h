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
		bool AddBlockToStack(TileType eTile, int nTileIndex, int nInvIndex);
		std::pair<bool, int> HasItemInInventory(ItemType eItem);
		bool HasBlockInInventory(TileType eTile, int& nInvIndex);
		bool RemoveItemFromInventory(std::shared_ptr<Item> eItem);

		static bool AddItemFromInventoryToOther(std::shared_ptr<Inventory> mFirstInv, std::shared_ptr<Inventory> mSecondInv, int nFirstIndex, int nSecondIndex);
	private:
	};
	//class Storage : public Entity, public Inventory //Base class för saker med ett inventory
	//{
	//	Storage
	//};
}



