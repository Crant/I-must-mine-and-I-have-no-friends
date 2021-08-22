#pragma once
#include "olcPixelGameEngine.h";
#include <algorithm>

enum class TileType
{
	Empty,
	Dirt,
	Stone
};

class World
{
public:
	World(int width, int height, TileType* newWorld, std::string name);
	~World();

	static void SetTile(int index, TileType value);
	static void SetTile(olc::vf2d pos, TileType value);
	static TileType GetTile(int index);
	static TileType GetTile(int x, int y);
	static TileType GetTile(olc::vf2d pos);
	static int GetWidth();
	static int GetHeight();
	static int GetSize();
	static int Index(float x, float y);
	static int Index(olc::vf2d pos);
	static olc::vi2d Coordinates(int flatIndex);
	static bool IsInside(olc::vf2d tile, olc::vf2d check);
	static bool IsInside(int tileIndex, olc::vf2d check);
	static bool IsBlock(olc::vf2d pos);
	static bool IsBlock(float x, float y);


private:
	static TileType* nWorld;
	static std::string sWorldName;

	static int nWidth;
	static int nHeight;
	static int nSize;

};

