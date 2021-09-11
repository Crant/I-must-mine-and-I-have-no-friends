#pragma once
#include "olcPixelGameEngine.h";
#include "../Tiles.h"

namespace IMM 
{


	class World
	{
	public:
		//World(int width, int height, TileType* newWorld, std::string newName);
		World(){}
		~World();

		static World* Main()
		{
			if (Instance == nullptr)
				Instance = new World();

			return Instance;
		}
		void SetWorld(int width, int height, Tile* newWorld, std::string name);
		void SetTile(int index, TileType value);
		void SetTile(float x, float y, TileType value);
		void SetTile(olc::vf2d pos, TileType value);
		void SetTileGeneration(int index, TileType value);
		TileType* GetTile(int index);
		TileType* GetTile(int x, int y);
		TileType* GetTile(olc::vf2d pos);
		TileNeighbours GetNbour(int index);
		TileNeighbours GetNbour(int x, int y);
		TileNeighbours GetNbour(olc::vf2d pos);
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
		void SetNeighbours(float x, float y);
		void SetNeighbours(olc::vf2d pos);
		void SetNeighbours(int index);
		std::shared_ptr<std::vector<int>> FloodFill(int x, int y);
		void RemoveRegions();
		std::shared_ptr<std::vector<std::vector<int>>> GetRegions();
		void CreateRegion(std::vector<int>& vRegionTiles);

	private:
		 static inline World* Instance;
		 Tile* nWorld;
		 std::string sWorldName;
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

