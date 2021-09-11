#pragma once
#include "World.h"

namespace IMM 
{
	class GridGenerator
	{
	public:

		GridGenerator(const std::string& name, int width, int height);
		~GridGenerator();
		void GenerateWorld();
		void RandomFillMap();
		//int FillArea(int x, int y, float nPseudoRandNumber);	
		void SmoothMap();
		int GetSurroundingWallCount(int gridX, int gridY);
		void GenerateNeighbours();
		void PerlinTry();
		void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float* fOutput);

		void CreateWorld();
		void CreateCave(int nWidth, int nHeight, int nPosX, int nPosY);
		
	private:
		std::string sName;
		int width;
		int height;
		int pseudoRandomGenerator;
		const int nFillPercentage = 55;

		float* nHeightWalkPre = nullptr;
		float* nHeightWalkPerlin = nullptr;
		TileType* nTemporaryArray = nullptr;
	};
}