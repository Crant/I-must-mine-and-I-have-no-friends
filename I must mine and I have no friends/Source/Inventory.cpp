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
	//bool bFoundFirstEmpty = false;
	//int nFirstEmptySlot = 0;
	//for (int i = 0; i < mStorage.size(); i++)
	//{
	//	if (mStorage[i] == nullptr)
	//	{
	//		if (!bFoundFirstEmpty)
	//		{
	//			nFirstEmptySlot = i;
	//			bFoundFirstEmpty = true;
	//		}
	//	}
	//	else if (mStorage[i]->GetType() == cItem->GetType() && mStorage[i]->IsStackable())
	//	{
	//		if (mStorage[i]->GetTile() == cItem->GetTile())
	//		{
	//			//mStorage[i]->nAmount += cItem->nAmount;
	//			mStorage[i]->
	//			return true;
	//		}
	//	}
	//}
	//if (bFoundFirstEmpty)
	//{
	//	mStorage[nFirstEmptySlot] = cItem;
	//	return true;
	//}
	//return false;

	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i] == nullptr)
		{
			mStorage[i] = cItem;
			return true;
		}
	}
	return false;
}
bool IMM::Inventory::AddItemToInventoryAtPos(std::shared_ptr<Item> cItem, int nNewPos, int nOldPos)
{
	if (mStorage[nNewPos] == nullptr && mStorage[nOldPos] == nullptr)
	{
		mStorage[nNewPos] = cItem;
		//if (nOldPos >= 0 && nOldPos < mInventory.size())
		//{
		//	mInventory[nOldPos] = nullptr;
		//}
		return true;
	}
	else if (mStorage[nNewPos] == nullptr && mStorage[nOldPos] != nullptr)
	{
		mStorage[nNewPos] = cItem;
		mStorage[nOldPos] = nullptr;
	}
	else if (nOldPos >= 0 && nOldPos < mStorage.size())
	{
		std::swap(mStorage[nNewPos], mStorage[nOldPos]);
		return true;
	}
	return false;
}
bool IMM::Inventory::AddBlockToStack(TileType eTile, int nTileIndex ,int nInvIndex)
{
	auto eBlock = std::reinterpret_pointer_cast<Block>(mStorage[nInvIndex]);
	eBlock->nBlockList.push_back(World::Main()->GetPerlinSeed(nTileIndex));
	return true;
}
std::pair<bool, int> IMM::Inventory::HasItemInInventory(ItemType cItem)
{
	std::pair<bool, int> pResult;
	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i]->GetType() == cItem)
		{
			pResult = std::make_pair(true, i);
			return pResult;
		}
	}
	pResult = std::make_pair(false, 0);
	return pResult;
}
bool IMM::Inventory::HasBlockInInventory(TileType eTile, int& nInvIndex)
{
	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i] != nullptr &&  mStorage[i]->GetTile() == eTile)
		{
			nInvIndex = i;
			return true;
		}
	}
	return false;
}
bool Inventory::RemoveItemFromInventory(std::shared_ptr<Item> cItem)
{
	auto it = mStorage.begin();
	for (int i = 0; i < mStorage.size(); i++)
	{
		if (mStorage[i] == cItem)
		{
			//mStorage.erase(it + i);
			//mStorage.push_back(nullptr);
			mStorage[i] = nullptr;
			return true;
		}
	}
	return false;
}

bool IMM::Inventory::AddItemFromInventoryToOther(std::shared_ptr<Inventory> mFirstInv, std::shared_ptr<Inventory> mSecondInv, int nFirstIndex, int nSecondIndex)
{
	//if (mSecondInv->mStorage[nSecondIndex] == nullptr)
	//{
	//	 std::swap(mSecondInv->mStorage[nSecondIndex], mFirstInv->mStorage[nFirstIndex]);
	//	//if (nOldPos >= 0 && nOldPos < mInventory.size())
	//	//{
	//	//	mInventory[nOldPos] = nullptr;
	//	//}
	//	return true;
	//}
	//else if (mStorage[nNewPos] == nullptr && mStorage[nOldPos] != nullptr)
	//{
	//	mStorage[nNewPos] = cItem;
	//	mStorage[nOldPos] = nullptr;
	//}
	if (nSecondIndex >= 0 && nSecondIndex < mSecondInv->mStorage.size())
	{
		std::swap(mSecondInv->mStorage[nSecondIndex], mFirstInv->mStorage[nFirstIndex]);
		return true;
	}
	return false;
}

//bool IMM::Inventory::AddItemToInventoryStatic(std::shared_ptr<Item> eItem, int nNewPos, int nOldPos)
//{
//	if (mStorage[nNewPos] == nullptr && mStorage[nOldPos] == nullptr)
//	{
//		mStorage[nNewPos] = cItem;
//		//if (nOldPos >= 0 && nOldPos < mInventory.size())
//		//{
//		//	mInventory[nOldPos] = nullptr;
//		//}
//		return true;
//	}
//	else if (mStorage[nNewPos] == nullptr && mStorage[nOldPos] != nullptr)
//	{
//		mStorage[nNewPos] = cItem;
//		mStorage[nOldPos] = nullptr;
//	}
//	else if (nOldPos >= 0 && nOldPos < mStorage.size())
//	{
//		std::swap(mStorage[nNewPos], mStorage[nOldPos]);
//		return true;
//	}
//	return false;
//}
