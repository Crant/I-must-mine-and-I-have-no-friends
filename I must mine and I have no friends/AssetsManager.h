#pragma once
#include "Tiles.h"
#include "Include/olcPixelGameEngine.h"
using namespace IMM;

namespace IMM
{
	class Assets
	{
	public:
		static Assets& get()
		{
			static Assets instance;
			return instance;
		}

		Assets(Assets const&) = delete;
		void operator=(Assets const&) = delete;

		//olc::Sprite* GetSprite(TileType tile)
		//{
		//	return mTiles[tile];
		//}
		olc::Decal* GetSpriteDecal(TileType* tile)
		{
			return mTiles[*tile];
		}
		olc::Sprite* GetTileSheet()
		{
			return sTileSheet;
		}

		void LoadSprites();
	private:
		Assets() {}
		~Assets() {}

	private:
		std::unordered_map<TileType, olc::Decal*> mTiles;
		olc::Sprite* sprDemo = nullptr;
		olc::Sprite* sTileSheet = nullptr;
	};
}


