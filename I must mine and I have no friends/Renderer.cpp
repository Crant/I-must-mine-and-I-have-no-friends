#include "GameEngine.h"
#include "NetworkMessages.h"
#include "WorldEvents.h"
#include "safe.h"
#include "AssetsManager.h"
#include "MenuEvents.h"

using namespace IMM;

void GameEngine::Render()
{
	//CheckWrapping(mCamera.x, mCamera.x);

	mOffsetX = mCamera.x - (float)mVisibleTiles.x;
	mOffsetY = mCamera.y - (float)mVisibleTiles.y;

	float fTileOffsetX = (mOffsetX - (int)mOffsetX) * mTileSize;
	float fTileOffsetY = (mOffsetY - (int)mOffsetY) * mTileSize;
	//float fTileOffsetX = (mOffsetX - mOffsetX) * mTileSize;
	//float fTileOffsetY = (mOffsetY - mOffsetY) * mTileSize;

	for (int x = -1; x < mVisibleTiles.x + 1; x++)
	{
		for (int y = -1; y < mVisibleTiles.y + 1; y++)
		{
			float ox = (float)x + mOffsetX;

			CheckWrapping(ox, ox);

			if (mWorld->IsBlock(ox, y + mOffsetY))
			{

				TileType* tile = mWorld->GetTile(olc::vf2d(ox, y + mOffsetY));
				//int tileNbour = (int)mWorld->GetNbour(olc::vf2d(ox, y + mOffsetY));
				int tileNbour = CheckNeighbour(ox, y + mOffsetY, true);
				olc::vf2d pos = olc::vf2d(x * mTileSize - fTileOffsetX, y * mTileSize - fTileOffsetY);


				
				

				DrawPartialDecal(
					pos,
					Assets::Main()->GetSpriteDecal(tile),
					olc::vi2d(0, mPixelSize * tileNbour),
					olc::vi2d(mPixelSize, mPixelSize),
					olc::vf2d(mTileScale, mTileScale));
			}
		}
	}
	auto ptr = AllBlocks::GetBlockYield(nPerlinBlocks[(int)mMousePos.x * 1024 + (int)mMousePos.y]);
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), ptr->sCommonYield + " Common yield", olc::WHITE, olc::vf2d(0.5f, 0.5f));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), ptr->sGoodUncommonYield + " Good uncommon yield", olc::YELLOW, olc::vf2d(0.5f, 0.5f));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), ptr->sBadUncommonYield + " Bad uncommon yield", olc::GREEN, olc::vf2d(0.5f, 0.5f));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 40), ptr->sGoodRareYield + " Good rare yield", olc::RED, olc::vf2d(0.5f, 0.5f));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 50), ptr->sBadRareYield + " Bad rare yield", olc::BLACK, olc::vf2d(0.5f, 0.5f));
	//Block blockAtPos(nBlockSeeds[(int)mMousePos.x * 1024 + (int)mMousePos.y]);
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(blockAtPos.nCoalAmount) + " Coal");
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(blockAtPos.nIronAmount) + " Iron");
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), std::to_string(blockAtPos.nCopperAmount) + " Copper");
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 40), std::to_string(blockAtPos.nQuartzAmount) + " Quartz");
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 60), std::to_string(nPerlinBlocks[(int)mMousePos.x * 1024 + (int)mMousePos.y]));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), std::to_string(mMousePos.x));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY()), std::to_string((int)mWorld->GetTile(mCamera.x, mCamera.y)));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY()), std::to_string(12));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(nuigger));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY()), std::to_string(mMousePos.x));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(mMousePos.y));
}
void GameEngine::CheckWrapping(int ix, int& ox)
{
	ox = ix;

	if (ix < 0.0f)
	{
		ox = ix + World::Main()->GetWidth();
	}
	if (ix > World::Main()->GetWidth())
	{
		ox = ix - World::Main()->GetWidth();
	}
}
void GameEngine::CheckWrapping(float ix, float& ox)
{
	ox = ix;

	if (ix < 0.0f)
	{
		ox = ix + World::Main()->GetWidth();
	}
	if (ix > World::Main()->GetWidth())
	{
		ox = ix - World::Main()->GetWidth();
	}
}
int GameEngine::CheckNeighbour(int x, int y, bool recursive)
{
	int nTileNeighbours = 0;
	int xPosCheck = x;

	CheckWrapping(x, x);

	if (mWorld->IsBlock(x, y + 1))
	{
		nTileNeighbours += 1;
	}
	CheckWrapping(x + 1, xPosCheck);
	if (mWorld->IsBlock(xPosCheck, y))
	{
		//xPosCheck = x + 1;
		//CheckWrapping(xPosCheck, xPosCheck);
		nTileNeighbours += 2;
	}
	//if (mWorld->IsBlock(x + 1, y))
	//{
	//	//xPosCheck = x + 1;
	//	//CheckWrapping(xPosCheck, xPosCheck);
	//	nTileNeighbours += 2;
	//}
	//xPosCheck = x;
	if (mWorld->IsBlock(x, y - 1))
	{
		nTileNeighbours += 4;
	}
	CheckWrapping(x - 1, xPosCheck);
	if (mWorld->IsBlock(xPosCheck, y))
	{
		//xPosCheck = x - 1;
		//CheckWrapping(xPosCheck, xPosCheck);
		nTileNeighbours += 8;
	}
	//if (mWorld->IsBlock(x - 1, y))
	//{
	//	//xPosCheck = x - 1;
	//	//CheckWrapping(xPosCheck, xPosCheck);
	//	nTileNeighbours += 8;
	//}

	return nTileNeighbours;
}
void GameEngine::InitCameraSettings()
{
	mVisibleTiles.x = ScreenWidth() / mTileSize;
	mVisibleTiles.y = ScreenHeight() / mTileSize;

	mTileScale = ((float)mTileSize / (float)mPixelSize);
}