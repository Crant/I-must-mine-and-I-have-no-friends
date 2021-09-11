#include "../I must mine and I have no friends/Renderer.h"
#include "Include/Game.h"

using namespace IMM;

void Renderer::MoveCamera()
{
	fMousePosX = ((float)Game::Main()->GetMouseX() / tileSize) + fOffsetX; //HUR MAN FÅR TAG I MUSPEKAREN I WORLDSPACE FRÅN SKÄRM
	fMousePosY = ((float)Game::Main()->GetMouseY() / tileSize) + fOffsetY;

	if (Game::Main()->IsFocused())
	{
		Game::Main()->DrawStringDecal(olc::vf2d(Game::Main()->GetMouseX() + 10, Game::Main()->GetMouseY()), std::to_string(fCameraPosX));
		Game::Main()->DrawStringDecal(olc::vf2d(Game::Main()->GetMouseX() + 10, Game::Main()->GetMouseY() + 10), std::to_string(fCameraPosY));
		if (Cooler != nullptr)
		{
			Game::Main()->DrawStringDecal(olc::vf2d(Game::Main()->GetMouseX() + 10, Game::Main()->GetMouseY() - 10), std::to_string(Cooler->size()));
		}

		if (Game::Main()->GetMouse(1).bHeld && !(World::Main()->IsBlock(fMousePosX, fMousePosY)))
		{
			
			TileController::CreateBlock(olc::vf2d(fMousePosX, fMousePosY), TileType::Dirt);
		}
		//if (Game::Main()->GetMouse(0).bHeld && World::Main()->IsBlock(fMousePosX, fMousePosY) && fTimer > 0.2f)
		//{
		//	Cool = World::Main()->FloodFill(fMousePosX, fMousePosY, Cool);
		//	//TileController::DamageBlock(World::Main()->Coordinates(World::Main()->Index(fMousePosX, fMousePosY)), 0.5f);
		//	fTimer = 0.0f;
		//}
		if (Game::Main()->GetMouse(0).bHeld)
		{
			//Cooler = World::Main()->GetRegions(fMousePosX, fMousePosY);
			//Cooler = World::Main()->GetRegions();
			World::Main()->RemoveRegions();
		}
		if (Game::Main()->GetKey(olc::Key::W).bHeld)
		{
			fCameraPosY -= fCameraSpeed * Game::Main()->GetElapsedTime();
		}
		if (Game::Main()->GetKey(olc::Key::A).bHeld)
		{
			fCameraPosX -= fCameraSpeed * Game::Main()->GetElapsedTime();
		}
		if (Game::Main()->GetKey(olc::Key::D).bHeld)
		{
			fCameraPosX += fCameraSpeed * Game::Main()->GetElapsedTime();
		}
		if (Game::Main()->GetKey(olc::Key::S).bHeld)
		{
			fCameraPosY += fCameraSpeed * Game::Main()->GetElapsedTime();
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

	//if (fOffsetX < 0) fOffsetX = 0;
	//if (fOffsetY < 0) fOffsetY = 0;
	//if (fOffsetX > World::Main()->GetWidth() - nVisibleTilesX) fOffsetX = World::Main()->GetWidth() - nVisibleTilesX;
	//if (fOffsetY > World::Main()->GetHeight() - nVisibleTilesY) fOffsetY = World::Main()->GetHeight() - nVisibleTilesY;



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
	if (fCameraPosX + 40 < 0) fCameraPosX = 40;
	if (fCameraPosY + 40 < 0) fCameraPosY = 40;
	if (fCameraPosX > World::Main()->GetWidth()) fCameraPosX = World::Main()->GetWidth();
	if (fCameraPosY > World::Main()->GetHeight()) fCameraPosY = World::Main()->GetHeight();
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

