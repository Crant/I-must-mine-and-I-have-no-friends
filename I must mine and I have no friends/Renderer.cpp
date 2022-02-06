#include "GameEngine.h"
#include "NetworkMessages.h"
#include "WorldEvents.h"
#include "safe.h"
#include "AssetsManager.h"
#include "MenuEvents.h"
#include "Include/Globals.h"

using namespace IMM;
//using namespace Col;

void GameEngine::Render()
{
	//mOffsetX = mCamera.x - (float)mVisibleTiles.x;
	//mOffsetY = mCamera.y - (float)mVisibleTiles.y;

	//float fTileOffsetX = (mOffsetX - (int)mOffsetX) * mTileSize;
	//float fTileOffsetY = (mOffsetY - (int)mOffsetY) * mTileSize;

	olc::vf2d vTileOffset = { (GlobalState::GetCamOffset().x - (int)GlobalState::GetCamOffset().x) * GlobalState::GetTileSize() ,
		(GlobalState::GetCamOffset().y - (int)GlobalState::GetCamOffset().y) * GlobalState::GetTileSize() };

	for (int x = -1; x < GlobalState::GetVisTiles().x + 1; x++)
	{
		for (int y = -1; y < GlobalState::GetVisTiles().y + 1; y++)
		{
			//float ox = (float)x + mOffsetX;

			//CheckWrapping(ox, ox);

			//if (mWorld->IsBlock(ox, y + mOffsetY))
			//{

			//	TileType tile = mWorld->GetTile(olc::vf2d(ox, y + mOffsetY));
			//	int tileNbour = CheckNeighbour(ox, y + mOffsetY, true);
			//	olc::vf2d pos = olc::vf2d(x * mTileSize - fTileOffsetX, y * mTileSize - fTileOffsetY);

			//	DrawPartialDecal(
			//		pos,
			//		Assets::Main()->GetSpriteDecal(tile),
			//		olc::vi2d(0, mPixelSize * tileNbour),
			//		olc::vi2d(mPixelSize, mPixelSize),
			//		olc::vf2d(mTileScale, mTileScale));
			float ox = (float)x + GlobalState::GetCamOffset().x;

			CheckWrapping(ox, ox);

			if (mWorld->IsBlock(ox, y + GlobalState::GetCamOffset().y))
			{

				TileType tile = mWorld->GetTile(olc::vf2d(ox, y + GlobalState::GetCamOffset().y));
				int tileNbour = CheckNeighbour(ox, y + GlobalState::GetCamOffset().y, true);
				olc::vf2d pos = olc::vf2d(x * GlobalState::GetTileSize() - vTileOffset.x, y * GlobalState::GetTileSize() - vTileOffset.y);

				DrawPartialDecal(
					pos,
					Assets::Main()->GetSpriteDecal(tile),
					olc::vi2d(0, GlobalState::GetPixelSize() * tileNbour),
					olc::vi2d(GlobalState::GetPixelSize(), GlobalState::GetPixelSize()),
					olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
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
	int inViewCount = 0;
	if (cObjects.size() > 0)
	{

		for (auto& obj : cObjects)
		{
			if (GlobalState::IsInView(obj->vPos))
			{
				obj->DrawSelf();

				inViewCount++;
			}
		}
	}


	for (auto flag : World::Main()->vBlockFlags)
	{
		DrawPartialDecal(GlobalState::WorldToScreen(flag), Assets::Main()->GetSpriteDecal(TileType::Debug), olc::vi2d(0, GlobalState::GetPixelSize() * 15),
			olc::vi2d(GlobalState::GetPixelSize(), GlobalState::GetPixelSize()),
			olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
	}
	World::Main()->vBlockFlags.clear();


	olc::vf2d meme = Col::RayVsWorld(ray_point, ray_direction.norm(), Maths::Distance(ray_point, olc::vf2d(mMousePos.x, mMousePos.y)));

	DrawLineDecal(olc::vf2d((float)(ScreenWidth() / 2), (float)(ScreenHeight() / 2)), olc::vf2d((float)GetMouseX(), (float)GetMouseY()), olc::GREEN);
	DrawCircle(Maths::WorldToScreen(meme, mTileSize, olc::vf2d(mOffsetX, mOffsetY)), 4.f, olc::RED);
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(cObjects.size()));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(inViewCount));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(mTempPlayer->vPos.x));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), std::to_string(mTempPlayer->vPos.y));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(GlobalState::GetMouseWorld().x));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(GlobalState::GetMouseWorld().y));
	DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), std::to_string(World::Main()->IsBlock(GlobalState::GetMouseWorld().x, GlobalState::GetMouseWorld().y)));
}
void GameEngine::StringDebugger()
{

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