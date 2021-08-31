#include "../I must mine and I have no friends/Renderer.h"
#include "Include/Game.h"

using namespace IMM;

void Renderer::MoveCamera()
{
	fMousePosX = ((float)Game::Main()->GetMouseX() / tileSize) + fOffsetX; //HUR MAN FÅR TAG I MUSPEKAREN I WORLDSPACE FRÅN SKÄRM
	fMousePosY = ((float)Game::Main()->GetMouseY() / tileSize) + fOffsetY;
	

	if (Game::Main()->IsFocused())
	{
		Game::Main()->DrawStringDecal(olc::vf2d(Game::Main()->GetMouseX() + 10, Game::Main()->GetMouseY()), std::to_string(TileController::nActiveTiles.size()));
		if (Game::Main()->GetMouse(1).bHeld && !(World::Main()->IsBlock(fMousePosX, fMousePosY)))
		{
			TileController::CreateBlock(olc::vf2d(fMousePosX, fMousePosY), TileType::Dirt);
		}
		if (Game::Main()->GetMouse(0).bHeld && World::Main()->IsBlock(fMousePosX, fMousePosY) && fTimer > 0.2f)
		{
			TileController::DamageBlock(World::Main()->Coordinates(World::Main()->Index(fMousePosX, fMousePosY)), 0.5f);
			fTimer = 0.0f;
		}
		if (Game::Main()->GetKey(olc::Key::W).bHeld)
		{
			fCameraPosY += 10.0f * Game::Main()->GetElapsedTime();
		}
		if (Game::Main()->GetKey(olc::Key::A).bHeld)
		{
			fCameraPosX -= 10.0f * Game::Main()->GetElapsedTime();
		}
		if (Game::Main()->GetKey(olc::Key::D).bHeld)
		{
			fCameraPosX += 10.0f * Game::Main()->GetElapsedTime();
		}
		if (Game::Main()->GetKey(olc::Key::S).bHeld)
		{
			fCameraPosY -= 10.0f * Game::Main()->GetElapsedTime();
		}

	}
	fTimer += Game::Main()->GetElapsedTime();
}
void Renderer::RenderCamera()
{
	fOffsetX = fCameraPosX - (float)nVisibleTilesX;
	fOffsetY = fCameraPosY - (float)nVisibleTilesY;

	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * tileSize;
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * tileSize;


	for (int x = -1; x < nVisibleTilesX + 1; x++)
	{
		for (int y = -1; y < nVisibleTilesY + 1; y++)
		{
			if (World::Main()->IsBlock(x + fOffsetX, y + fOffsetY))
			{
				TileType* tile = World::Main()->GetTile(olc::vf2d(x + fOffsetX, y + fOffsetY));
				int tileNbour = (int)World::Main()->GetNbour(olc::vf2d(x + fOffsetX, y + fOffsetY));
				olc::vf2d pos = olc::vf2d(x * tileSize - fTileOffsetX, y * tileSize - fTileOffsetY);

				Game::Main()->DrawPartialDecal(pos, Assets::get().GetSpriteDecal(tile), olc::vi2d(0, pixelSize * tileNbour), olc::vi2d(pixelSize, pixelSize), olc::vf2d(tileScale, tileScale));
			}
		}
	}
}
void Renderer::UpdateCamera()
{
	RenderCamera();
	MoveCamera();
}
void Renderer::SetCamera()
{
	//this->pge = World::get();
	nVisibleTilesX = Game::Main()->ScreenWidth() / tileSize;
	nVisibleTilesY = Game::Main()->ScreenHeight() / tileSize;


	 //sDirt = Assets::get().Get(TileType::Dirt);
	 //dDirt = new olc::Decal(sDirt);
	 tileScale = ((float)tileSize / (float)pixelSize);
}

