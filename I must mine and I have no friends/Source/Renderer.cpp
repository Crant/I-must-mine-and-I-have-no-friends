#include "Renderer.h"
#include "GameEngine.h"

using namespace IMM;

void Renderer::MoveCamera()
{
	fMousePosX = ((float)GameEngine::Main()->GetMouseX() / tileSize) + fOffsetX; //HUR MAN FÅR TAG I MUSPEKAREN I WORLDSPACE FRÅN SKÄRM
	fMousePosY = ((float)GameEngine::Main()->GetMouseY() / tileSize) + fOffsetY;

	if (GameEngine::Main()->IsFocused())
	{
		GameEngine::Main()->DrawStringDecal(olc::vf2d(GameEngine::Main()->GetMouseX() + 10, GameEngine::Main()->GetMouseY()), std::to_string(fCameraPosX));
		GameEngine::Main()->DrawStringDecal(olc::vf2d(GameEngine::Main()->GetMouseX() + 10, GameEngine::Main()->GetMouseY() + 10), std::to_string(fCameraPosY));
		if (Cooler != nullptr)
		{
			GameEngine::Main()->DrawStringDecal(olc::vf2d(GameEngine::Main()->GetMouseX() + 10, GameEngine::Main()->GetMouseY() - 10), std::to_string(Cooler->size()));
		}

		if (GameEngine::Main()->GetMouse(1).bHeld && !(mWorld->IsBlock(fMousePosX, fMousePosY)))
		{
			
			TileController::CreateBlock(olc::vf2d(fMousePosX, fMousePosY), TileType::Dirt);
		}
		//if (Game::Main()->GetMouse(0).bHeld && World::Main()->IsBlock(fMousePosX, fMousePosY) && fTimer > 0.2f)
		//{
		//	Cool = World::Main()->FloodFill(fMousePosX, fMousePosY, Cool);
		//	//TileController::DamageBlock(World::Main()->Coordinates(World::Main()->Index(fMousePosX, fMousePosY)), 0.5f);
		//	fTimer = 0.0f;
		//}
		if (GameEngine::Main()->GetMouse(0).bHeld)
		{
			//Cooler = World::Main()->GetRegions(fMousePosX, fMousePosY);
			//Cooler = World::Main()->GetRegions();
			mWorld->RemoveRegions();
		}
		if (GameEngine::Main()->GetKey(olc::Key::W).bHeld)
		{
			fCameraPosY -= fCameraSpeed * GameEngine::Main()->GetElapsedTime();
		}
		if (GameEngine::Main()->GetKey(olc::Key::A).bHeld)
		{
			fCameraPosX -= fCameraSpeed * GameEngine::Main()->GetElapsedTime();
		}
		if (GameEngine::Main()->GetKey(olc::Key::D).bHeld)
		{
			fCameraPosX += fCameraSpeed * GameEngine::Main()->GetElapsedTime();
		}
		if (GameEngine::Main()->GetKey(olc::Key::S).bHeld)
		{
			fCameraPosY += fCameraSpeed * GameEngine::Main()->GetElapsedTime();
		}

	}
	fTimer += GameEngine::Main()->GetElapsedTime();
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
			if (mWorld->IsBlock(x + fOffsetX, y + fOffsetY))
			{
				TileType* tile = mWorld->GetTile(olc::vf2d(x + fOffsetX, y + fOffsetY));
				int tileNbour = (int)mWorld->GetNbour(olc::vf2d(x + fOffsetX, y + fOffsetY));
				olc::vf2d pos = olc::vf2d(x * tileSize - fTileOffsetX, y * tileSize - fTileOffsetY);

				GameEngine::Main()->DrawPartialDecal(
					pos, 
					Assets::Main()->GetSpriteDecal(tile), 
					olc::vi2d(0, pixelSize * tileNbour), 
					olc::vi2d(pixelSize, pixelSize), 
					olc::vf2d(tileScale, tileScale));
			}
		}
	}
	if (fCameraPosX + 40 < 0) 
		fCameraPosX = 40;

	if (fCameraPosY + 40 < 0) 
		fCameraPosY = 40;

	if (fCameraPosX > mWorld->GetWidth()) 
		fCameraPosX = mWorld->GetWidth();

	if (fCameraPosY > mWorld->GetHeight()) 
		fCameraPosY = mWorld->GetHeight();
}
void Renderer::UpdateCamera()
{
	RenderCamera();
	MoveCamera();
}
void Renderer::SetCamera()
{
	nVisibleTilesX = GameEngine::Main()->ScreenWidth() / tileSize;
	nVisibleTilesY = GameEngine::Main()->ScreenHeight() / tileSize;
	
	 //sDirt = Assets::get().Get(TileType::Dirt);
	 //dDirt = new olc::Decal(sDirt);
	 tileScale = ((float)tileSize / (float)pixelSize);
}

