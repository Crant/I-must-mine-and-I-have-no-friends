#include "Item.h"
#include "World.h"
#include "Globals.h"
#include "EntityManager.h"

using namespace IMM;

IMM::Item::Item() 
	:sName(""), eItem(ItemType::Default), sDescription("N/A"), bIsStackable(false), bIsUpdated(false), eTile(TileType::Empty), nAmount(1)
{

}

Item::Item(const std::string& sName, ItemType eItem)
	: sDescription("N/A"), bIsStackable(false), bIsUpdated(false), eTile(TileType::Empty), nAmount(1)
{
	this->sName = sName;
	this->eItem = eItem;
}
Item::Item(const std::string& sName, ItemType eItem, bool bIsUpdated)
	: sDescription("N/A"), bIsStackable(false), eTile(TileType::Empty), nAmount(1)
{
	this->sName = sName;
	this->eItem = eItem;
	this->bIsUpdated = bIsUpdated;
}
Item::Item(const std::string& sName, ItemType eItem, bool bIsStackable, bool bIsUpdated)
	: sDescription("N/A"), eTile(TileType::Empty), nAmount(1)
{
	this->sName = sName;
	this->eItem = eItem;
	this->bIsStackable = bIsStackable;
	this->bIsUpdated = bIsUpdated;
}
ItemType IMM::Item::GetType()
{
	return eItem;
}
bool IMM::Item::IsStackable()
{
	return bIsStackable;
}
TileType IMM::Item::GetTile()
{
	return eTile;
}
//bool Item::UpdateSelf()
//{
//	return true;
//}

///////
///////
///////
///////
bool MiningLaser::Use(PhysUnit* cItemHolder)
{
	olc::vf2d vRangeCheck = cItemHolder->vTarget - cItemHolder->vPos;
	if (std::abs(vRangeCheck.x) > 5 || std::abs(vRangeCheck.y) > 5)
	{
		return true;
	}
	World::Main()->DamageBlock(cItemHolder->vTarget, 10.f * GlobalState::GetEngine()->GetElapsedTime());
	return true;
}
bool PhysObjShooter::Use(PhysUnit* cItemHolder)
{
	olc::vf2d vDir = cItemHolder->vTarget - cItemHolder->vPos;
	std::shared_ptr<IMM::PhysProjectile> mTempPlayer = std::make_shared<IMM::PhysProjectile>(
		cItemHolder->vPos, olc::vf2d(1.f, 1.f), SpriteType::Default, vDir.norm() * 70.f,
		100.f);
	EntityManager::AddObj(mTempPlayer);
	return true;
}
///
///
///
/// 
/// 
/// 
/// 
/// 
//IMM::Block::Block(const std::string& sName, TileType eTile, int nAmount) 
//	: Item(sName, ItemType::Block, true, true)
//{
//	this->eTile = eTile;
//	this->nAmount = nAmount;
//}
IMM::Block::Block(TileType eTile, int index, int nAmount) :
	Item("Block", ItemType::Block, true, true)
{
	this->eTile = eTile;
	nBlockList.push_back(World::Main()->GetPerlinSeed(index));
}
IMM::Block::Block(TileType eTile, int index) :
	Item("Block", ItemType::Block, true, true)
{
	this->eTile = eTile;
	nBlockList.push_back(World::Main()->GetPerlinSeed(index));
}
bool IMM::Block::Use(PhysUnit* cItemHolder)
{
	if (nBlockList.size() > 0 && !World::Main()->IsBlock(cItemHolder->vTarget))
	{
		World::Main()->CreateBlock(cItemHolder->vTarget, eTile, nBlockList.front());
		nBlockList.pop_front();
		if (nBlockList.size() <= 0)
		{
			return false;
		}
	}
	return true;
}
bool IMM::Block::UpdateSelf()
{
	if (nBlockList.size() <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
//void Block::Use(PhysUnit* cItemHolder)
//{
//	if (nMaxStackSize <= 0)
//	{
//		cItemHolder->
//	}
//}

//IMM::Block::Block()
//{
//	
//}
