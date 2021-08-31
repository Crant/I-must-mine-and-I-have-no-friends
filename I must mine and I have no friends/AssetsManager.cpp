#include "AssetsManager.h"

void Assets::LoadSprites()
{
	olc::Sprite* lolDirt = new olc::Sprite("./Resources/Assets/Dirt.png");
	mTiles[TileType::Dirt] = new olc::Decal(lolDirt);
}