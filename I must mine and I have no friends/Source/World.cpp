#include "../Include/World.h"

using namespace IMM;


//World::World(int width, int height, TileType* newWorld, std::string newName)
//{
//    nWidth = width;
//    nHeight = height;
//    nWorld = newWorld;
//    sWorldName = newName;
//    Instance = this;
//}
//World* World::GetWorld()
//{
//    return Instance;
//}

World::~World()
{
    delete[] nWorld;
    delete Instance;
}
void World::SetWorld(int width, int height, Tile* newWorld, std::string newName)
{
    nWidth = width;
    nHeight = height;
    nSize = width * height;
    nWorld = newWorld;
    sWorldName = newName;
    Instance = this;
}
void World::SetTile(int index, TileType value)
{
    nWorld[index].type = value;
    SetNeighbours(index);
}
void World::SetTile(float x, float y, TileType value)
{
    nWorld[Index(x, y)].type = value;
    SetNeighbours(x, y);
}
void World::SetTile(olc::vf2d pos, TileType value)
{
    nWorld[Index(pos)].type = value;
    SetNeighbours(pos);
}
void World::SetTileGeneration(int index, TileType value) //USE ONLY IN GENERATION 
{
    nWorld[index].type = value;
}

TileType* World::GetTile(int index)
{
    return &nWorld[index].type;
}
TileType* World::GetTile(int x, int y)
{
    return &nWorld[Index(x, y)].type;
}
TileType* World::GetTile(olc::vf2d pos)
{
    return &nWorld[Index(pos)].type;
}
TileNeighbours World::GetNbour(int index)
{
    return nWorld[std::clamp(index, 0, nHeight * nWidth - 1)].Nbrs;
}
TileNeighbours World::GetNbour(int x, int y)
{
    return nWorld[Index(x, y)].Nbrs;
}
TileNeighbours World::GetNbour(olc::vf2d pos)
{
    return nWorld[Index(pos)].Nbrs;
}
int World::GetWidth()
{
    return nWidth;
}
int World::GetHeight()
{
    return nHeight;
}
int World::GetSize()
{
    return nSize;
}

std::string World::GetName()
{
    return sWorldName;
}
int World::Index(float x, float y)
{
    return std::clamp(nHeight * (int)x + (int)y, 0, (nWidth * nHeight) - 1);
}
int World::Index(olc::vf2d pos)
{
    return std::clamp(nHeight * (int)pos.x + (int)pos.y, 0, (nWidth * nHeight) - 1);
}
olc::vi2d World::Coordinates(int flatIndex)
{
    return olc::vi2d(flatIndex / nHeight, flatIndex % nHeight);
}
bool World::IsInside(olc::vf2d tile, olc::vf2d check)
{
    return check.x >= tile.x && check.y >= tile.y && check.x < tile.x + 1 && check.y < tile.y + 1;
}
bool World::IsInside(int tileIndex, olc::vf2d check)
{
    olc::vi2d tile = Coordinates(tileIndex);
    return check.x >= tile.x && check.y >= tile.y && check.x < tile.x + 1 && check.y < tile.y + 1;
}
//bool World::IsOverlap(olc::vf2d tileOne, olc::vf2d tileTwo)
//{
//    return tileOne.x < tileTwo.x + 1 && tileOne.x + 1 > tileTwo.x && tileOne.y < tileTwo.y + 1 && tileOne.y + 1 > tileTwo.y;
//}
bool World::IsBlock(olc::vf2d pos)
{
    return (int)nWorld[Index(pos)].type >= 1;
}

bool World::IsBlock(float x, float y)
{
    return (int)nWorld[Index(x, y)].type >= 1;
}
bool World::IsBlock(int index)
{
    return (int)nWorld[std::clamp(index, 0, nWidth * nHeight)].type >= 1;
}
void World::SetNeighbours(float x, float y)
{
    int nTileNeighbours = 0;
    if (IsBlock(x, y + 1) > 0)
    {
        nTileNeighbours += 1;
        nWorld[Index(x, y + 1)].Nbrs = SetNeighboursNotRecursive(x, y + 1);
    }
    if (IsBlock(x + 1, y) > 0)
    {
        nTileNeighbours += 2;
        nWorld[Index(x + 1, y)].Nbrs = SetNeighboursNotRecursive(x + 1, y);
    }
    if (IsBlock(x, y - 1) > 0)
    {
        nTileNeighbours += 4;
        nWorld[Index(x, y - 1)].Nbrs = SetNeighboursNotRecursive(x, y - 1);
    }
    if (IsBlock(x - 1, y) > 0)
    {
        nTileNeighbours += 8;
        nWorld[Index(x - 1, y)].Nbrs = SetNeighboursNotRecursive(x - 1, y);
    }
    nWorld[Index(x, y)].Nbrs = (TileNeighbours)nTileNeighbours;
}
void World::SetNeighbours(olc::vf2d pos)
{
    int nTileNeighbours = 0;
    if (IsBlock(pos.x, pos.y + 1) > 0)
    {
        nTileNeighbours += 1;
        nWorld[Index(pos.x, pos.y + 1)].Nbrs = SetNeighboursNotRecursive(pos.x, pos.y + 1);
    }
    if (IsBlock(pos.x + 1, pos.y) > 0)
    {
        nTileNeighbours += 2;
        nWorld[Index(pos.x + 1, pos.y)].Nbrs = SetNeighboursNotRecursive(pos.x + 1, pos.y);
    }
    if (IsBlock(pos.x, pos.y - 1) > 0)
    {
        nTileNeighbours += 4;
        nWorld[Index(pos.x, pos.y - 1)].Nbrs = SetNeighboursNotRecursive(pos.x, pos.y - 1);
    }
    if (IsBlock(pos.x - 1, pos.y) > 0)
    {
        nTileNeighbours += 8;
        nWorld[Index(pos.x - 1, pos.y)].Nbrs = SetNeighboursNotRecursive(pos.x - 1, pos.y);
    }
    nWorld[Index(pos)].Nbrs = (TileNeighbours)nTileNeighbours;
}
void World::SetNeighbours(int index)
{
    int nTileNeighbours = 0;
    if (IsBlock(index + 1) > 0)
    {
        nTileNeighbours += 1;
        nWorld[index + 1].Nbrs = SetNeighboursNotRecursive(index + 1);
    }
    if (IsBlock(index + nWidth) > 0)
    {
        nTileNeighbours += 2;
        nWorld[index + nWidth].Nbrs = SetNeighboursNotRecursive(index + nWidth);
    }
    if (IsBlock(index - 1) > 0)
    {
        nTileNeighbours += 4;
        nWorld[index - 1].Nbrs = SetNeighboursNotRecursive(index - 1);
    }
    if (IsBlock(index - nWidth) > 0)
    {
        nTileNeighbours += 8;
        nWorld[index - nWidth].Nbrs = SetNeighboursNotRecursive(index - nWidth);
    }
    nWorld[index].Nbrs = (TileNeighbours)nTileNeighbours;
}
TileNeighbours World::SetNeighboursNotRecursive(float x, float y)
{
    int nTileNeighbours = 0;
    if (IsBlock(x, y + 1) > 0)
    {
        nTileNeighbours += 1;
    }
    if (IsBlock(x + 1, y) > 0)
    {
        nTileNeighbours += 2;
    }
    if (IsBlock(x, y - 1) > 0)
    {
        nTileNeighbours += 4;
    }
    if (IsBlock(x - 1, y) > 0)
    {
        nTileNeighbours += 8;
    }
    return (TileNeighbours)nTileNeighbours;
}
TileNeighbours World::SetNeighboursNotRecursive(olc::vf2d pos)
{
    int nTileNeighbours = 0;
    if (IsBlock(pos.x, pos.y + 1) > 0)
    {
        nTileNeighbours += 1;
    }
    if (IsBlock(pos.x + 1, pos.y) > 0)
    {
        nTileNeighbours += 2;
    }
    if (IsBlock(pos.x, pos.y - 1) > 0)
    {
        nTileNeighbours += 4;
    }
    if (IsBlock(pos.x - 1, pos.y) > 0)
    {
        nTileNeighbours += 8;
    }
    return (TileNeighbours)nTileNeighbours;
}
TileNeighbours World::SetNeighboursNotRecursive(int index)
{
    int nTileNeighbours = 0;
    if (IsBlock(index + 1) > 0)
    {
        nTileNeighbours += 1;
    }
    if (IsBlock(index + nWidth) > 0)
    {
        nTileNeighbours += 2;
    }
    if (IsBlock(index - 1) > 0)
    {
        nTileNeighbours += 4;
    }
    if (IsBlock(index - nWidth) > 0)
    {
        nTileNeighbours += 8;
    }
    return (TileNeighbours)nTileNeighbours;
}
