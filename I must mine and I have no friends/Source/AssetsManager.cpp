#include "AssetsManager.h"

using namespace IMM;

void Assets::LoadSprites()
{
	//Loading Tiles
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

	olc::Sprite* Debug = new olc::Sprite("Assets/Debug.png");
	mTiles[TileType::Debug] = new olc::Decal(Debug);

	//Loading Entities

	olc::Sprite* defaultGuy = new olc::Sprite("Assets/GubbWithArm.png");
	mEntities[SpriteType::Default] = new olc::Decal(defaultGuy);
}
Assets* Assets::Main()
{
	if (Instance == nullptr)
		Instance = new Assets();

	return Instance;
}
olc::Decal* Assets::GetSpriteDecal(TileType tile)
{
	return mTiles[tile];
}
olc::Decal* Assets::GetSpriteDecal(SpriteType entity)
{
	return mEntities[entity];
}
olc::Sprite* Assets::GetTileSheet()
{
	return sTileSheet;
}
Assets::Assets()
{

}
Assets::~Assets()
{

}