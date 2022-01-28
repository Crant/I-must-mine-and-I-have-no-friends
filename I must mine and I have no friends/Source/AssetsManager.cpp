#include "AssetsManager.h"

void IMM::Assets::LoadSprites()
{
	olc::Sprite* lolDirt = new olc::Sprite("Assets/Dirt.png");
	mTiles[TileType::Dirt] = new olc::Decal(lolDirt);

	olc::Sprite* Clay = new olc::Sprite("Assets/Clay.png");
	mTiles[TileType::Clay] = new olc::Decal(Clay);

	olc::Sprite* Anthracite = new olc::Sprite("Assets/Anthracite.png");
	mTiles[TileType::Anthracite] = new olc::Decal(Anthracite);

	olc::Sprite* Granite = new olc::Sprite("Assets/Granite.png");
	mTiles[TileType::Granite] = new olc::Decal(Granite);

	olc::Sprite* Basalt = new olc::Sprite("Assets/Basalt.png");
	mTiles[TileType::Basalt] = new olc::Decal(Basalt);
}