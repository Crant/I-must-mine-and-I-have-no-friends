#pragma once
#include "../Include/World.h"

namespace IMM 
{
	class GridGenerator
	{
	public:

		GridGenerator(std::string name, int width, int height);
		~GridGenerator();
		TileType* GenerateWorld();
		void RandomFillMap();
		//int FillArea(int x, int y, float nPseudoRandNumber);	
		void SmoothMap();
		int GetSurroundingWallCount(int gridX, int gridY);
		
	private:
		std::string sName;
		int width;
		int height;
		TileType* newWorld;
	};
}