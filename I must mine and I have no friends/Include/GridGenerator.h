#pragma once
#include "../Include/World.h"

namespace IMM 
{
	class GridGenerator
	{
	public:

		GridGenerator(std::string name, int width, int height, olc::PixelGameEngine* pge);
		~GridGenerator();
		void GenerateWorld();
		void RandomFillMap();
		//int FillArea(int x, int y, float nPseudoRandNumber);	
		void SmoothMap();
		int GetSurroundingWallCount(int gridX, int gridY);
		void GenerateNeighbours();
		World* GetWorld();
		
	private:
		std::string sName;
		int width;
		int height;
		const int nFillPercentage = 50;
		World* sWorldToBeGenerated;
	};
}