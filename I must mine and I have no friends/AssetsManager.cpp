#include "AssetsManager.h"

void Assets::LoadSprites()
{
	//auto load = [&](std::string sName, std::string sFileName)
	//{
	//	olc::Sprite* s = new olc::Sprite(sFileName);
	//	mSprites[sName] = s;
	//};
	//load("Tiles", "./Resources/Assets/TileSheet");
	//sTileSheet = new olc::Sprite("./Resources/Assets/Dirt.png");

	sLoadedSprites[TileType::Dirt] = new olc::Sprite("./Resources/Assets/Dirt.png");
	mTiles[TileType::Dirt] = new olc::Decal(sLoadedSprites[TileType::Dirt]);
}