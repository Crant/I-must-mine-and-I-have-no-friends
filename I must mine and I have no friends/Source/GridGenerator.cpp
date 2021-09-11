#include "GridGenerator.h"
#include "RandomGen.h"

using namespace IMM;

GridGenerator::GridGenerator(const std::string& name, int width, int height)
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

    World::Main()->Init(width, height, new Tile[worldSize], sName);

    nHeightWalkPre = new float[width];
    nHeightWalkPerlin = new float[width];
    nTemporaryArray = new TileType[worldSize];

    for (size_t h = 0; h < width; h++)
    {
        nHeightWalkPre[h] = ((float)rand() / (float)RAND_MAX);
    }
    nHeightWalkPre[0] = 0.5f;
    PerlinNoise1D(width, nHeightWalkPre, 8, nHeightWalkPerlin);

    GenerateWorld();
    GenerateNeighbours();
}
void GridGenerator::PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float* fOutput)
{
    for (int x = 0; x < nCount; x++)
    {
        float fScale = 1.0f;
        float fNoise = 0.0f;
        float fScaleAcc = 0.0f;
        for (int y = 0; y < nOctaves; y++)
        {
            int nPitch = nCount >> y;
            int nSample1 = (x / nPitch) * nPitch;
            int nSample2 = (nSample1 + nPitch) % nCount;

            float fBlend = (float)(x - nSample1) / (float)nPitch;
            float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
            fNoise += fSample * fScale;
            fScaleAcc += fScale;

            fScale = fScale / 2.0f;
        }
        fOutput[x] = (fNoise / fScaleAcc) * height;
    }
}
GridGenerator::~GridGenerator() 
{

}

void GridGenerator::PerlinTry()
{
    //int* heightMap = new int[width];
    //int heightHalf = height / 2;
    //for (size_t i = 0; i < width; i++)
    //{
    //    float fScale = 1.0f;
    //    float fNoise = 0.0f;
    //    float fScaleAcc = 0.0f;
    //    for (size_t o = 0; o < 5; o++)
    //    {
    //        int pitch = width >> o;

    //    }
    //    heightMap[i] = ((float)rand() / (float)RAND_MAX) * heightHalf;
    //}
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
    CreateWorld();
    CreateCave(width, height, 0, 0);
    World::Main()->RemoveRegions();
    //for (size_t i = 0; i < 5; i++)
    //{
    //    SmoothMap();
    //}
    delete[] nTemporaryArray;
}
void GridGenerator::CreateWorld()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            if (y < nHeightWalkPerlin[x])
            {
                nTemporaryArray[World::Main()->Index(x, y)] = TileType::Empty;
                World::Main()->SetTileGeneration(x * height + y, TileType::Empty);
            }
            else
            {
                nTemporaryArray[World::Main()->Index(x, y)] = TileType::Dirt;
                World::Main()->SetTileGeneration(x * height + y, TileType::Dirt);
            }

        }
    }
}
void GridGenerator::CreateCave(int nWidth, int nHeight, int nPosX, int nPosY)
{
    for (int x = nPosX; x < nWidth + nPosX; x++)
    {
        for (int y = nPosY; y < nHeight + nPosY; y++)
        {
            if (World::Main()->IsBlock(x, y))
            {
                if (x == 0 || x == nWidth - 1 || y == 0 || y == nHeight - 1)
                {
                    nTemporaryArray[World::Main()->Index(x, y)] = TileType::Dirt;
                    World::Main()->SetTileGeneration(x * height + y, TileType::Dirt);
                }
                else
                {
                    int nRand = Utils::Random::Rand() % 100;
                    nTemporaryArray[World::Main()->Index(x, y)] = nRand < (nFillPercentage) ? TileType::Dirt : TileType::Empty;
                    World::Main()->SetTileGeneration(x * height + y, (nRand < (nFillPercentage) ? TileType::Dirt : TileType::Empty));
                }
            }

        }
    }
    for (size_t i = 0; i < 5; i++)
    {
        SmoothMap();
    }
}
void GridGenerator::RandomFillMap()
{
    Utils::Random::Srand(sName);
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            //[x * height + y].type = rand() % 100 < nFillPercentage ? TileType::Dirt : TileType::Empty;
            if (y < nHeightWalkPerlin[x])
            {
                //sWorldToBeGenerated->SetTileGeneration(x * height + y, rand() % 100 < nFillPercentage ? TileType::Dirt : TileType::Empty);
                nTemporaryArray[World::Main()->Index(x, y)] = TileType::Empty;
                World::Main()->SetTileGeneration(x * height + y, TileType::Empty);
            }
            else
            {
                //sWorldToBeGenerated->SetTileGeneration(x * height + y, TileType::Dirt);
                int nRand = Utils::Random::Rand() % 100;
                nTemporaryArray[World::Main()->Index(x, y)] = nRand < (nFillPercentage) ? TileType::Dirt : TileType::Empty;
                World::Main()->SetTileGeneration(x * height + y, (nRand < (nFillPercentage)? TileType::Dirt : TileType::Empty));
            }

        }
    }
}

void GridGenerator::SmoothMap()
{
    int worldSize = width * height;
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {
            int neighbourWallTiles = GetSurroundingWallCount(x, y);

            if (neighbourWallTiles > 4)
            {
                nTemporaryArray[World::Main()->Index(x, y)] = TileType::Dirt;
                //sWorldToBeGenerated->SetTileGeneration(x * height + y, TileType::Dirt);
            }

            else if (neighbourWallTiles < 4)
            {
                nTemporaryArray[World::Main()->Index(x, y)] = TileType::Empty;
                //sWorldToBeGenerated->SetTileGeneration(x * height + y, TileType::Empty);
            }

        }
    }
    for (size_t i = 0; i < worldSize; i++)
    {
        World::Main()->SetTileGeneration(i, nTemporaryArray[i]);
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
                    if (World::Main()->IsBlock(neighbourX * height + neighbourY))
                        wallCount += 1;
                }
            }

        }
    }

    return wallCount;
}