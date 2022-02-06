#pragma once
#include "Tiles.h"
#include "olcPixelGameEngine.h"
#include "Player.h"

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
		olc::Sprite* GetTileSheet();
		void LoadSprites();

	private:
		Assets();
		~Assets();

	private:
		static inline Assets* Instance;

		std::unordered_map<TileType, olc::Decal*> mTiles;
		std::unordered_map<SpriteType, olc::Decal*>mEntities;
		olc::Sprite* sprDemo = nullptr;
		olc::Sprite* sTileSheet = nullptr;
	};
}