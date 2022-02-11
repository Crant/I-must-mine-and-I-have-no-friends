#pragma once
#include "Tiles.h"
#include "olcPixelGameEngine.h"
#include "PhysicsObject.h"
#include"Item.h"

namespace IMM
{
	class Assets
	{
	public:
		static Assets* Main();
		Assets(Assets const&) = delete;
		void operator=(Assets const&) = delete;
		olc::Decal* GetSpriteDecal(TileType tile);
		olc::Decal* GetSpriteDecal(SpriteType entity);
		olc::Decal* GetSpriteDecal(ItemType item);
		olc::Sprite* GetTileSheet();
		void LoadSprites();

	private:
		Assets();
		~Assets();

	private:
		static inline Assets* Instance;

		std::unordered_map<TileType, olc::Decal*> mTiles;
		std::unordered_map<SpriteType, olc::Decal*>mEntities;
		std::unordered_map<ItemType, olc::Decal*>mItems;
		olc::Sprite* sprDemo = nullptr;
		olc::Sprite* sTileSheet = nullptr;
	};
}