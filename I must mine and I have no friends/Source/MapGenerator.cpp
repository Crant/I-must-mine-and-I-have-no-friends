#include "../MapGenerator.h"


MapGenerator::MapGenerator()
{
	width = 0;
	height = 0;

	nHeightWalkPre = nullptr;
	nHeightWalkPerlin = nullptr;
	nTemporaryArray = nullptr;	
}
IMM::MapGenerator::~MapGenerator()
{
}
void ChangeMap()
{
	//if (World::Main() != nullptr)
	//{
	//	//Save map to disk and 
	//}
}
void MapGenerator::GenerateMap(const std::string& seed, int nWidth, int nHeight, MapType eMap, float fGravity)
{
	auto fPerlinBlockTypes = Maths::Get2DPerlinNoise(seed, nWidth, nHeight); // HÄR SKAPAS PERLIN BLOCKSEN 

	mWorld = std::make_shared<World>(nWidth, nHeight, new Tile[nWidth * nHeight], fGravity, fPerlinBlockTypes);

	float* nPerlinWalk = new float[nWidth * nHeight];		      //Perlin noise För ytan av planeten
	nPerlinWalk = Maths::Get1DPerlinNoise(seed, nWidth, nHeight); //Måste initializas utanför switch casen för nån jävla anledning       //Perlin noise i 2D som bestämmer vilka sorters block som ska placeras ut vart


	switch (eMap)
	{
	case MapType::EarthlikePlanet:

		for (size_t x = 0; x < nWidth; x++)
		{
			for (size_t y = 0; y < nHeight; y++)
			{

				if (y < nPerlinWalk[x] || fPerlinBlockTypes->at(x * nHeight + y) < 0.45f)							//Fyller i endast om värdet är lägre än Y
				{																								//
					mWorld->SetTile(x * nHeight + y, TileType::Empty);											//
				}
				else
				{
					//float fModified = (float)y / ((float)nHeight);
					//fModified = 0.2f * fModified;

					float fModified = Maths::CalculateRange((float)y, nHeight / 2, 0.f, 0.f, 1.f, Maths::SmoothStop);

					mWorld->SetTile(x * nHeight + y, CalculateBlockType(fPerlinBlockTypes->at(x * nHeight + y), MapType::EarthlikePlanet));
				}
			}
		}


		break;
	case MapType::MagmaPlanet:
		break;
	case MapType::AsteroidField:
		break;
	default:
		break;
	}
	delete[] nPerlinWalk;
	IMM::Events::WorldCreatedEvent WCE(mWorld, fPerlinBlockTypes);

	NotifyObservers(&WCE);

	//return fPerlinBlockTypes;
}

TileType MapGenerator::CalculateBlockType(float nPerlinValue, MapType map) //Antagligen deprecated
{
	//nPerlinValue = nPerlinValue * nPerlinValue;
	switch (map)
	{
	case IMM::MapType::EarthlikePlanet:



		if (nPerlinValue > 0.8f)
		{
			return TileType::Anthracite;
		}
		else if (nPerlinValue > 0.7f)
		{
			return TileType::Basalt;
		}
		else if (nPerlinValue > 0.5f)
		{
			return TileType::Granite;
		}
		else if (nPerlinValue > 0.4f)
		{
			return TileType::Clay;
		}
		else
		{
			return TileType::Dirt;
		}
		//if (nPerlinValue > 0.52f)
		//{
		//	return TileType::Anthracite;
		//}
		//else if (nPerlinValue > 0.51f)
		//{
		//	return TileType::Dirt;
		//}
		//else if (nPerlinValue > 0.5f)
		//{
		//	return TileType::Granite;
		//}
		//else if (nPerlinValue > 0.49f)
		//{
		//	return TileType::Clay;
		//}
		//else
		//{
		//	return TileType::Basalt;
		//}

		break;
	case IMM::MapType::MagmaPlanet:
		break;
	case IMM::MapType::AsteroidField:
		break;
	default:
		return TileType::Empty;
		break;
	}
}
