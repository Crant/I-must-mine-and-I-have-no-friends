#pragma once
#include "../Include/World.h"

namespace IMM 
{
	class GridGenerator
	{
	public:

		GridGenerator(std::string name);
		~GridGenerator();
		TileType* GenerateWorld(int width, int height);
		TileType* RandomFillMap(TileType* world, int width, int height);
		//int FillArea(int x, int y, float nPseudoRandNumber);	
		//TileType* SmoothMap(TileType* world, int width, int height);
		//int GetSurroundingWallCount(int gridX, int gridY, TileType* world, int width, int height);
		
	private:
		std::string sName;
		
	};
}