#include "../Include/GridGenerator.h"
#include "../Include/World.h"
using namespace IMM;



GridGenerator::GridGenerator(std::string name, int width, int height)
{
    if (name == "")
    {
        
    }
    else
    {
        sName = name;
    }
    this->width = width;
    this->height = height;
    int worldSize = width * height;

    newWorld = new TileType[worldSize];
}
GridGenerator::~GridGenerator() 
{

}

TileType* GridGenerator::GenerateWorld()
{
    RandomFillMap();
    for (size_t i = 0; i < 4; i++)
    {
        SmoothMap();
    }
    return newWorld;
}
void GridGenerator::RandomFillMap()
{
    int pseudoRandomGenerator = std::hash<std::string>()(sName);
    srand(pseudoRandomGenerator);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            newWorld[x * height + y] = rand() % 100 < 50 ? TileType::Dirt : TileType::Empty;
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
                newWorld[x * height + y] = TileType::Dirt;
            }

            else if (neighbourWallTiles < 4)
            {
                newWorld[x * height + y] = TileType::Empty;
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
                    wallCount += 1;
                }
            }

        }
    }

    return wallCount;
}


