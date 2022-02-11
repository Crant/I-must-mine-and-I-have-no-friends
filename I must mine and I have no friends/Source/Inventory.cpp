#include "Inventory.h"

IMM::Inventory::Inventory(int nStorageSpace) 
{
	mStorage = std::vector<std::shared_ptr<Item>>(nStorageSpace);
}
IMM::Inventory::Inventory() :
	mStorage(std::vector<std::shared_ptr<Item>>(64))
{

}

bool Inventory::AddItemToInventory(std::shared_ptr<Item> cItem)
{
	bool bFoundFirstEmpty = false;
	int nFirstEmptySlot = 0;
	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i] == nullptr)
		{
			if (!bFoundFirstEmpty)
			{
				nFirstEmptySlot = i;
				bFoundFirstEmpty = true;
			}
		}
		else if (mStorage[i]->GetType() == cItem->GetType() && mStorage[i]->IsStackable())
		{
			if (mStorage[i]->GetTile() == cItem->GetTile())
			{
				mStorage[i]->nAmount += cItem->nAmount;
				return true;
			}
			//else
			//{
			//	mStorage[i] = cItem;
			//}


		}
	}
	if (bFoundFirstEmpty)
	{
		mStorage[nFirstEmptySlot] = cItem;
		return true;
	}
	return false;
}
bool IMM::Inventory::AddItemToInventoryAtPos(std::shared_ptr<Item> cItem, int nNewPos, int nOldPos)
{
	if (mStorage[nNewPos] == nullptr)
	{
		mStorage[nNewPos] == cItem;
		//if (nOldPos >= 0 && nOldPos < mInventory.size())
		//{
		//	mInventory[nOldPos] = nullptr;
		//}
		return true;
	}
	else if (nOldPos >= 0 && nOldPos < mStorage.size())
	{
		std::swap(mStorage[nNewPos], mStorage[nOldPos]);
		return true;
	}
	return false;
}
bool IMM::Inventory::HasItemInInventory(ItemType cItem)
{
	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i]->GetType() == cItem)
		{
			return true;
		}
	}
	return false;
}
bool IMM::Inventory::HasBlockInInventory(TileType eTile)
{
	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i]->GetTile() == eTile)
		{
			return true;
		}
	}
	return false;
}
bool Inventory::RemoveItemFromInventory(std::shared_ptr<Item> cItem)
{
	return false;
}
