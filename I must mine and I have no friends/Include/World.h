#pragma once

#include "olcPixelGameEngine.h";
#include "Tiles.h"
#include "Observer.h"

namespace IMM 
{
	class World : public Observed
	{
	public:
		World(int width, int height, Tile* newWorld);
		World() { this->nWorld = nullptr; }

		~World();

		static World* Main()
		{
			if (Instance == nullptr)
				Instance = new World();

			return Instance;
		}
		void Init(int width, int height, Tile* newWorld);
		void SetTile(int index, TileType value);
		void SetTile(float x, float y, TileType value);
		void SetTile(olc::vf2d pos, TileType value);
		void SetTileGeneration(int index, TileType value);
		TileType GetTile(int index);
		TileType GetTile(int x, int y);
		TileType GetTile(olc::vf2d pos);
		TileNeighbours GetNbour(int index);
		TileNeighbours GetNbour(int x, int y);
		TileNeighbours GetNbour(olc::vf2d pos);

		inline Tile* GetWorld() { return nWorld; }

		int GetWidth();
		int GetHeight();
		int GetSize();
		int Index(float x, float y);
		int Index(olc::vf2d pos);

		olc::vi2d Coordinates(int flatIndex);
		bool IsInside(olc::vf2d tile, olc::vf2d check);
		bool IsInside(int tileIndex, olc::vf2d check);
		bool IsBlock(olc::vf2d pos);
		bool IsBlock(float x, float y);
		bool IsBlock(int index);

		void FlagBlock(int x, int y); //FOR DEBUGGING
		void FlagBlock(float x, float y);//FOR DEBUGGING
		void FlagBlock(const olc::vf2d& pos);//FOR DEBUGGING
		std::vector<olc::vi2d> vBlockFlags;//FOR DEBUGGING. Töm och fyll på egen hand

		void SetNeighbours(float x, float y);
		void SetNeighbours(olc::vf2d pos);
		void SetNeighbours(int index);
		std::shared_ptr<std::vector<int>> FloodFill(int x, int y);
		void RemoveRegions();
		std::shared_ptr<std::vector<std::vector<int>>> GetRegions();
		void CreateRegion(std::vector<int>& vRegionTiles);

		void CheckDamagedTiles();
		void DamageBlock(olc::vf2d blockPos, float dmg);
		void DamageBlockAOE(olc::vf2d blockPos, float dmg, float aoe);

		void CreateBlock(olc::vf2d blockPos, TileType tt);
		void CreateBlock(float blockXPos, float blockYPos, TileType tt);
		void CreateBlock(int index, TileType tt);
		void RemoveBlock(olc::vf2d blockPos);
		void RemoveBlock(float blockXPos, float blockYPos);
		void RemoveBlock(int index);

	private:
		 static inline World* Instance;
		 Tile* nWorld;
		 std::unordered_map<int, TileData> mDamagedTiles;
		 //std::shared_ptr<std::vector<int>> nFloodFill = std::make_shared<std::vector<int>>();

		 int nWidth = 0;
		 int nHeight = 0;
		 int nSize = 0;

		 TileNeighbours SetNeighboursNotRecursive(float x, float y);
		 TileNeighbours SetNeighboursNotRecursive(olc::vf2d pos);
		 TileNeighbours SetNeighboursNotRecursive(int index);

		 //std::vector<Region> cRegions;
	};

	class Region
	{
	public:
		Region(std::vector<int>& vRegionTiles);
		~Region();
		std::vector<int> vRegionTiles;
		std::vector<int>* vEdgeTiles;
		std::vector<Region>* vConnectedRegions;

		static void ConnectRegions(Region& cRegionA, Region& cRegionB);
		bool IsConnected(Region& cOtherRegion);
	private:

	};
}

