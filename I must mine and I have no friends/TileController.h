//#pragma once
//#include "Include/olcPixelGameEngine.h"
//#include "Include/World.h"
//using namespace IMM;
//
//namespace IMM
//{
//	class TileController
//	{
//		std::vector<int>* nKeys;
//		std::unordered_map<int, TileData>* nActiveTiles;
//		const float fResetTimer = 3.0f;
//		TileData sTileData;
//
//		olc::PixelGameEngine* pge;
//	public:
//		TileController();
//
//		void UpdateBlocks();
//
//		void CheckActiveTiles();
//		void DestroyBlock(olc::vf2d fBlockPos, float fMineSpd);
//		void CreateBlock(olc::vf2d fBlockPos, TileType tBlock);
//
//		TileData GetTileData(int key);
//		void SetTileData(int key, TileData tile);
//		void DeactivateTileData(int key);
//		void ActivateTileData(int key, TileData tile);
//		std::unordered_map<int, TileData> GetActiveTiles();
//	};
//}