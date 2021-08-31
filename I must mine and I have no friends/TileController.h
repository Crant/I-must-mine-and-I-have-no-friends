#pragma once
#include "Include/olcPixelGameEngine.h"
#include "Include/World.h"
using namespace IMM;

namespace IMM
{
	class TileController
	{
		olc::PixelGameEngine* pge;
	public:
		TileController();
		~TileController(){}
		void UpdateBlocks();
		static inline std::unordered_map<int, TileData> nActiveTiles;

		static void CheckActiveTiles();
		static void DamageBlock(olc::vf2d fBlockPos, float fDmg);
		static void DamageBlockAOE(olc::vf2d fBlockPos, float fDmg, int nAoe);
		static void CreateBlock(olc::vf2d fBlockPos, TileType tBlock);

		TileData GetTileData(int key);
		void SetTileData(int key, TileData tile);
		void DeactivateTileData(int key);
		void ActivateTileData(int key, TileData tile);
		std::unordered_map<int, TileData> GetActiveTiles();
	};
}