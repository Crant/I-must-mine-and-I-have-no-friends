#include "../Include/GridGenerator.h"
#include "../Include/RandomGen.h"

using namespace IMM;

GridGenerator::GridGenerator(std::string name, int width, int height)
{
    if (name == "")
    {
        int nRandomSeed = rand();
        sName = std::to_string(nRandomSeed);
    }
    else
    {
        sName = name;
    }
    this->width = width;
    this->height = height;
    int worldSize = width * height;

    sWorldToBeGenerated = new World();
    sWorldToBeGenerated->SetWorld(width, height, new Tile[worldSize], sName);
    GenerateWorld();
    GenerateNeighbours();
}

GridGenerator::~GridGenerator() 
{

}

World* GridGenerator::GetWorld() 
{
    return sWorldToBeGenerated;
}

void GridGenerator::GenerateNeighbours()
{
    for (size_t x = 0; x < World::Main()->GetWidth(); x++)
    {
        for (size_t y = 0; y < World::Main()->GetHeight(); y++)
        {
            World::Main()->SetNeighbours(x, y);
        }
    }
}

void GridGenerator::GenerateWorld()
{
    RandomFillMap();
    for (size_t i = 0; i < 4; i++)
    {
        SmoothMap();
    }
}

void GridGenerator::RandomFillMap()
{
    //int pseudoRandomGenerator = std::hash<std::string>()(sName);
    //srand(pseudoRandomGenerator);

    Utils::Random::Srand(sName);
    
    //std::cout << "Seed: " << Utils::Random::randomGen. << "\n";

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            //[x * height + y].type = rand() % 100 < nFillPercentage ? TileType::Dirt : TileType::Empty;
            sWorldToBeGenerated->SetTileGeneration(x * height + y, Utils::Random::Rand() % 100 < nFillPercentage ? TileType::Dirt : TileType::Empty);
        }
    }
}

void GridGenerator::SmoothMap()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            int neighbourWallTiles = GetSurroundingWallCount(x, y);

            if (neighbourWallTiles > 4)
            {
                //newWorld[x * height + y].type = TileType::Dirt;
                sWorldToBeGenerated->SetTileGeneration(x * height + y, TileType::Dirt);
            }

            else if (neighbourWallTiles < 4)
            {
                //newWorld[x * height + y].type = TileType::Empty;
                sWorldToBeGenerated->SetTileGeneration(x * height + y, TileType::Empty);
            }

        }
    }
}

int GridGenerator::GetSurroundingWallCount(int gridX, int gridY)
{
    int wallCount = 0;
    for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX++)
    {
        for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY++)
        {
            if (neighbourX >= 0 && neighbourX < width && neighbourY >= 0 && neighbourY < height)
            {
                if (neighbourX != gridX || neighbourY != gridY)
                {
                    //wallCount += (int)newWorld[neighbourX * height + neighbourY].type;
                    if (sWorldToBeGenerated->IsBlock(neighbourX * height + neighbourY))
                        wallCount += 1;
                }
            }

        }
    }

    return wallCount;
}