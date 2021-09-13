#pragma once
#include "Tiles.h"
#include "olcPixelGameEngine.h"

namespace IMM
{
	class Assets
	{
	public:
		static Assets* Main()
		{
			if (Instance == nullptr)
				Instance = new Assets();

			return Instance;
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
		static inline Assets* Instance;

		std::unordered_map<TileType, olc::Decal*> mTiles;
		olc::Sprite* sprDemo = nullptr;
		olc::Sprite* sTileSheet = nullptr;
	};
}