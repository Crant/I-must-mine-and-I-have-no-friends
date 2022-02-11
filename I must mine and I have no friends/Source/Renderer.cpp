//#include "GameEngine.h"
//#include "NetworkMessages.h"
//#include "WorldEvents.h"
//#include "safe.h"
#include "AssetsManager.h"
//#include "MenuEvents.h"
#include "Globals.h"
#include "GraphicsRenderer.h"
#include "EntityManager.h"

//#include "World.h"

using namespace IMM;
using namespace GFX;
//using namespace Col;


void GFX::RenderSprites(IMM::Entity* sprite)
{
	GlobalState::GetEngine()->DrawPartialDecal(
		GlobalState::WorldToScreen(sprite->vPos),
		Assets::Main()->GetSpriteDecal(sprite->sModel),
		olc::vi2d(0, 0),
		olc::vi2d(sprite->vSize.x * GlobalState::GetPixelSize(), sprite->vSize.y * GlobalState::GetPixelSize()),
		olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
}
void GFX::RenderTiles()
{

	olc::vf2d vTileOffset = { (GlobalState::GetCamOffset().x - (int)GlobalState::GetCamOffset().x) * GlobalState::GetTileSize() ,
		(GlobalState::GetCamOffset().y - (int)GlobalState::GetCamOffset().y) * GlobalState::GetTileSize() };

	for (int x = -1; x < GlobalState::GetVisTiles().x + 1; x++)
	{
		for (int y = -1; y < GlobalState::GetVisTiles().y + 1; y++)
		{
			float ox = (float)x + GlobalState::GetCamOffset().x;

			World::Main()->CheckWrapping(ox, ox);

			if (World::Main()->IsBlock(ox, y + GlobalState::GetCamOffset().y))
			{

				TileType tile = World::Main()->GetTile(olc::vf2d(ox, y + GlobalState::GetCamOffset().y));
				int tileNbour = CheckNeighbour(ox, y + GlobalState::GetCamOffset().y);
				olc::vf2d pos = olc::vf2d(x * GlobalState::GetTileSize() - vTileOffset.x, y * GlobalState::GetTileSize() - vTileOffset.y);

				GlobalState::GetEngine()->DrawPartialDecal(
					pos,
					Assets::Main()->GetSpriteDecal(tile),
					olc::vi2d(0, GlobalState::GetPixelSize() * tileNbour),
					olc::vi2d(GlobalState::GetPixelSize(), GlobalState::GetPixelSize()),
					olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
			}
		}
	}
	//olc::vf2d vTileOffset = { (GlobalState::GetCamOffset().x - (int)GlobalState::GetCamOffset().x) * GlobalState::GetTileSize() ,
	//(GlobalState::GetCamOffset().y - (int)GlobalState::GetCamOffset().y) * GlobalState::GetTileSize() };

	//for (int x = -1; x < GlobalState::GetVisTiles().x + 1; x++)
	//{
	//	for (int y = -1; y < GlobalState::GetVisTiles().y + 1; y += 8)
	//	{
	//		float ox = (float)x + GlobalState::GetCamOffset().x;

	//		World::Main()->CheckWrapping(ox, ox);

	//		if (World::Main()->IsBlock(ox, y + GlobalState::GetCamOffset().y))
	//		{

	//			TileType tile = World::Main()->GetTile(olc::vf2d(ox, y + GlobalState::GetCamOffset().y));
	//			int tileNbour = CheckNeighbour(ox, y + GlobalState::GetCamOffset().y);
	//			olc::vf2d pos = olc::vf2d(x * GlobalState::GetTileSize() - vTileOffset.x, y * GlobalState::GetTileSize() - vTileOffset.y);

	//			GlobalState::GetEngine()->DrawPartialDecal(
	//				pos,
	//				Assets::Main()->GetSpriteDecal(tile),
	//				olc::vi2d(0, GlobalState::GetPixelSize() * tileNbour),
	//				olc::vi2d(GlobalState::GetPixelSize(), GlobalState::GetPixelSize()),
	//				olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
	//		}
	//	}
	//}

	TileDebugger();
	StringDebugger(std::to_string(EntityManager::AmountOfObjects()) + " Amount of objects in world");
}

void GFX::TileDebugger()
{
	for (auto flag : World::Main()->vBlockFlags)
	{
		GlobalState::GetEngine()->DrawPartialDecal(GlobalState::WorldToScreen(flag), Assets::Main()->GetSpriteDecal(TileType::Debug), olc::vi2d(0, GlobalState::GetPixelSize() * 15),
			olc::vi2d(GlobalState::GetPixelSize(), GlobalState::GetPixelSize()),
			olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
	}
	World::Main()->vBlockFlags.clear();
}
void GFX::StringDebugger(const std::string& sDebug)
{
	olc::vf2d vMousePos = GlobalState::GetEngine()->GetMousePos();
	GlobalState::GetEngine()->DrawStringDecal(olc::vf2d(vMousePos.x + 10, vMousePos.y), sDebug);
}
void GFX::StringSpam()
{
	//auto ptr = AllBlocks::GetBlockYield(nPerlinBlocks[(int)mMousePos.x * 1024 + (int)mMousePos.y]);
//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), ptr->sCommonYield + " Common yield", olc::WHITE, olc::vf2d(0.5f, 0.5f));
//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), ptr->sGoodUncommonYield + " Good uncommon yield", olc::YELLOW, olc::vf2d(0.5f, 0.5f));
//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), ptr->sBadUncommonYield + " Bad uncommon yield", olc::GREEN, olc::vf2d(0.5f, 0.5f));
//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 40), ptr->sGoodRareYield + " Good rare yield", olc::RED, olc::vf2d(0.5f, 0.5f));
//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 50), ptr->sBadRareYield + " Bad rare yield", olc::BLACK, olc::vf2d(0.5f, 0.5f));
//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 60), std::to_string(nPerlinBlocks[(int)mMousePos.x * 1024 + (int)mMousePos.y]));

	//olc::vf2d ray_point = Maths::LocalToWorld(olc::vf2d((float)(ScreenWidth() / 2.0f), (float)(ScreenHeight() / 2.0f)), mTileSize, olc::vf2d(mOffsetX, mOffsetY));

	//olc::vf2d mCameraReal = { mCamera.x, mCamera.y };
	//olc::vf2d ray_direction = { mMousePos.x - ray_point.x, mMousePos.y - ray_point.y };

	//TileDebugger();


	//olc::vf2d meme = Col::RayVsWorld(ray_point, ray_direction.norm(), Maths::Distance(ray_point, olc::vf2d(mMousePos.x, mMousePos.y)));

	//DrawLineDecal(olc::vf2d((float)(ScreenWidth() / 2), (float)(ScreenHeight() / 2)), olc::vf2d((float)GetMouseX(), (float)GetMouseY()), olc::GREEN);
	//DrawCircle(Maths::WorldToScreen(meme, mTileSize, olc::vf2d(mOffsetX, mOffsetY)), 4.f, olc::RED);
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(cObjects.size()) + " Amount of objects");
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string() + " Objects in view" );
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(mTempPlayer->bIsGrounded));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), std::to_string(mTempPlayer->vPos.y));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 10), std::to_string(GlobalState::GetMouseWorld().x));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 20), std::to_string(GlobalState::GetMouseWorld().y));
	//DrawStringDecal(olc::vf2d(GetMouseX() + 10, GetMouseY() + 30), std::to_string(World::Main()->IsBlock(GlobalState::GetMouseWorld().x, GlobalState::GetMouseWorld().y)));
}
int GFX::CheckNeighbour(int x, int y)
{
	int nTileNeighbours = 0;
	int xPosCheck = x;

	World::Main()->CheckWrapping(x, x);

	if (World::Main()->IsBlock(x, y + 1))
	{
		nTileNeighbours += 1;
	}
	World::Main()->CheckWrapping(x + 1, xPosCheck);
	if (World::Main()->IsBlock(xPosCheck, y))
	{
		nTileNeighbours += 2;
	}
	if (World::Main()->IsBlock(x, y - 1))
	{
		nTileNeighbours += 4;
	}
	World::Main()->CheckWrapping(x - 1, xPosCheck);
	if (World::Main()->IsBlock(xPosCheck, y))
	{
		nTileNeighbours += 8;
	}
	return nTileNeighbours;
}