#include "AssetsManager.h"

void Assets::LoadSprites()
{
	olc::Sprite* lolDirt = new olc::Sprite("Assets/Dirt.png");
	mTiles[TileType::Dirt] = new olc::Decal(lolDirt);
}