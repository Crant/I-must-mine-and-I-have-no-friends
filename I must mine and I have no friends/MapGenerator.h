#pragma once

#include "World.h"
#include "Observer.h"
#include <fstream>
#include "Include/WorldEvents.h"
#include "Maths.h"


using namespace IMM;
using namespace Maths;

namespace IMM
{
	enum class MapType : unsigned char
	{
		EarthlikePlanet,
		MagmaPlanet,
		AsteroidField,
	};

    class MapGenerator : public Observed
	{
	public:
		MapGenerator();
		~MapGenerator();
		void ChangeMap();
		void GenerateMap(const std::string& seed, int nWidth, int nHeight, MapType eMap, float fGravity);
	private:
		TileType CalculateBlockType(float nPerlinValue, MapType map); //Antagligen deprecated

		int width;
		int height;
		const int nFillPercentage = 55;

		float* nHeightWalkPre;
		float* nHeightWalkPerlin;
		TileType* nTemporaryArray;

		std::shared_ptr<World> mWorld;

	};
}
