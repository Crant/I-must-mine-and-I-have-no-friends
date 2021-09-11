#include "Tiles.h"

using namespace IMM;
void Tiles::LoadTiles()
{
	mTypeToData[TileType::Dirt] = TileData(1.0f, 1.0f);
	mTypeToData[TileType::Stone] = TileData(100.0f, 100.0f);
}
TileData Tiles::GetData(TileType* tile)
{
	return mTypeToData[*tile];
}