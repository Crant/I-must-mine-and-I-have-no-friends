#pragma once
#include "World.h"
#include "Observer.h"

namespace IMM 
{
	class GridGenerator : public Observed
	{
	public:
		GridGenerator();
		~GridGenerator();

		void Init(const std::string& seed, int width, int height);
		void GenerateWorld();
		void RandomFillMap();
		//int FillArea(int x, int y, float nPseudoRandNumber);	
		void SmoothMap();
		int GetSurroundingWallCount(int gridX, int gridY);
		int GetSurroundingTileTypes(int gridX, int gridY);
		void GenerateNeighbours();
		void PerlinTry();
		void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float* fOutput);

		void CreateWorld();
		void CreateCave(int nWidth, int nHeight, int nPosX, int nPosY);
		
	private:
		int width;
		int height;
		const int nFillPercentage = 55;

		float* nHeightWalkPre;
		float* nHeightWalkPerlin;
		TileType* nTemporaryArray;

		std::shared_ptr<World> mWorld;
	};
}