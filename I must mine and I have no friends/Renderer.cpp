#include "GameEngine.h"
#include "NetworkMessages.h"
#include "WorldEvents.h"
#include "safe.h"
#include "AssetsManager.h"
#include "MenuEvents.h"

using namespace IMM;
//using namespace Col;

void GameEngine::Render()
{
	mOffsetX = mCamera.x - (float)mVisibleTiles.x;
	mOffsetY = mCamera.y - (float)mVisibleTiles.y;

	float fTileOffsetX = (mOffsetX - (int)mOffsetX) * mTileSize;
	float fTileOffsetY = (mOffsetY - (int)mOffsetY) * mTileSize;

	for (int x = -1; x < mVisibleTiles.x + 1; x++)
	{
		for (int y = -1; y < mVisibleTiles.y + 1; y++)
		{
			float ox = (float)x + mOffsetX;

			CheckWrapping(ox, ox);

			if (mWorld->IsBlock(ox, y + mOffsetY))
			{

				TileType* tile = mWorld->GetTile(olc::vf2d(ox, y + mOffsetY));
				int tileNbour = CheckNeighbour(ox, y + mOffsetY, true);
				olc::vf2d pos = olc::vf2d(x * mTileSize - fTileOffsetX, y * mTileSize - fTileOffsetY);

				DrawPartialDecal(
					pos,
					Assets::Main()->GetSpriteDecal(tile),
					olc::vi2d(0, mPixelSize * tileNbour),
					olc::vi2d(mPixelSize, mPixelSize),
					olc::vf2d(mTileScale, mTileScale));


				//olc::Pixel col;
				//int pixel_bw = (int)(nPerlinBlocks[World::Main()->Index(pos)] * 12.0f);
				//switch (pixel_bw)
				//{
				//case 0: col = olc::BLACK; break;

				//case 1: col = olc::Pixel(32, 32, 32); break;
				//case 2: col = olc::VERY_DARK_GREY; break;
				//case 3: col = olc::Pixel(96, 96, 96); break;
				//case 4: col = olc::DARK_GREY; break;

				//case 5: col = olc::Pixel(144, 144, 144); break;
				//case 6: col = olc::Pixel(160, 160, 160); break;
				//case 7: col = olc::Pixel(176, 176, 176); break;
				//case 8: col = olc::GREY; break;

				//case 9:  col = col = olc::Pixel(208, 208, 208); break;
				//case 10: col = col = olc::Pixel(224, 224, 224); break;
				//case 11: col = olc::Pixel(240, 240, 240); break;
				//case 12: col = olc::WHITE; break;
				//}

				//Draw(x, y, col);
			}
		}
	}
	//auto ptr = AllBlocks::GetBlockYield(nPerlinBlocks[(int)mMousePos.x * 1024 + (int)mMousePos.y]);
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), ptr->sCommonYield + " Common yield", olc::WHITE, olc::vf2d(0.5f, 0.5f));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), ptr->sGoodUncommonYield + " Good uncommon yield", olc::YELLOW, olc::vf2d(0.5f, 0.5f));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), ptr->sBadUncommonYield + " Bad uncommon yield", olc::GREEN, olc::vf2d(0.5f, 0.5f));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 40), ptr->sGoodRareYield + " Good rare yield", olc::RED, olc::vf2d(0.5f, 0.5f));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 50), ptr->sBadRareYield + " Bad rare yield", olc::BLACK, olc::vf2d(0.5f, 0.5f));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 60), std::to_string(nPerlinBlocks[(int)mMousePos.x * 1024 + (int)mMousePos.y]));

	olc::vf2d ray_point = Maths::LocalToWorld(olc::vf2d((float)(ScreenWidth() / 2.0f), (float)(ScreenHeight() / 2.0f)), mTileSize, olc::vf2d(mOffsetX, mOffsetY));

	olc::vf2d mCameraReal = { mCamera.x, mCamera.y };
	olc::vf2d ray_direction = { mMousePos.x - ray_point.x, mMousePos.y - ray_point.y };
	//olc::vf2d ray_dirNorm = ray_direction.norm();
	//ray_direction = 

	

	olc::vf2d meme = Col::RayVsWorld(ray_point, ray_direction.norm(), Maths::Distance(ray_point, olc::vf2d(mMousePos.x, mMousePos.y)));

	DrawLineDecal(olc::vf2d((float)(ScreenWidth() / 2), (float)(ScreenHeight() / 2)), olc::vf2d((float)GetMouseX(), (float)GetMouseY()), olc::GREEN);
	DrawCircle(Maths::WorldToScreen(meme, mTileSize, olc::vf2d(mOffsetX, mOffsetY)), 4.f, olc::RED);
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(mCamera.x));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(mCamera.y));

	olc::vf2d cp, cn;
	float t;
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
		nTileNeighbours += 2;
	}
	if (mWorld->IsBlock(x, y - 1))
	{
		nTileNeighbours += 4;
	}
	CheckWrapping(x - 1, xPosCheck);
	if (mWorld->IsBlock(xPosCheck, y))
	{
		nTileNeighbours += 8;
	}
	return nTileNeighbours;
}
void GameEngine::InitCameraSettings()
{
	mVisibleTiles.x = ScreenWidth() / mTileSize;
	mVisibleTiles.y = ScreenHeight() / mTileSize;

	mTileScale = ((float)mTileSize / (float)mPixelSize);
}