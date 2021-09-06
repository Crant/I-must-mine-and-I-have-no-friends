#pragma once
#include "olcPixelGameEngine.h";
#include "../Tiles.h"

namespace IMM 
{
	struct WorldInfo
	{
		int width;
		int height;

		char seed[10];
	};

	class World
	{
	public:
		//World(int width, int height, TileType* newWorld, std::string newName);
		World() { this->nWorld = nullptr; }
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

		inline Tile* GetWorld() { return nWorld; }
		std::string GetName();

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

	private:
		 static inline World* Instance;
		 Tile* nWorld;
		 std::string sWorldName;

		 int nWidth = 0;
		 int nHeight = 0;
		 int nSize = 0;

		 TileNeighbours SetNeighboursNotRecursive(float x, float y);
		 TileNeighbours SetNeighboursNotRecursive(olc::vf2d pos);
		 TileNeighbours SetNeighboursNotRecursive(int index);
	};
}

