#include "../Include/World.h"


World::World(int width, int height, TileType* newWorld, std::string newName)
{
	nWidth = width;
	nHeight = height;
	nWorld = newWorld;
	sWorldName = newName;
};

World::~World()
{
	delete[] nWorld;
}

void World::SetTile(int index, TileType value)
{
    nWorld[index] = value;
}
void World::SetTile(olc::vf2d pos, TileType value)
{
    nWorld[Index(pos)] = value;
}
TileType World::GetTile(int index)
{
    return nWorld[index];
}
TileType World::GetTile(int x, int y)
{
    return nWorld[Index(x, y)];
}
TileType World::GetTile(olc::vf2d pos)
{
    return nWorld[Index(pos)];
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
int World::Index(float x, float y)
{
    return std::clamp(nHeight *  (int)x + (int)y, 0, (nWidth * nHeight) - 1);
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
    return (int)nWorld[Index(pos)] >= 1;
}

bool World::IsBlock(float x, float y)
{
    return (int)nWorld[Index(x, y)] >= 1;
}
