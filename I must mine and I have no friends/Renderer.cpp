#include "../I must mine and I have no friends/Renderer.h"
using namespace IMM;

void Renderer::MoveCamera(olc::PixelGameEngine* pge)
{
	fOffsetX = fCameraPosX - (float)nVisibleTilesX;
	fOffsetY = fCameraPosY - (float)nVisibleTilesY;

	float fTileOffsetX = (fOffsetX - (int)fOffsetX) * tileSize;
	float fTileOffsetY = (fOffsetY - (int)fOffsetY) * tileSize;

	//pge->DrawLine(olc::vi2d(0, 0), olc::vi2d(10, 10), olc::WHITE);
	//pge->DrawString(olc::vi2d(50, 50), std::to_string(pge->ScreenWidth()), olc::WHITE);

	//if (fOffsetX < 0)
	//{
	//	fOffsetX = 0;
	//}
	//if (fOffsetY < 0)
	//{
	//	fOffsetY = 0;
	//}
	//if (fOffsetX > World::GetWorld()->GetWidth() - nVisibleTilesX)
	//{
	//	fOffsetX = World::GetWorld()->GetWidth() - nVisibleTilesX;
	//}
	//if (fOffsetY < World::GetWorld()->GetHeight() - nVisibleTilesY)
	//{
	//	fOffsetY = World::GetWorld()->GetHeight() - nVisibleTilesY;
	//}

	for (int x = -1; x < nVisibleTilesX + 1; x++)
	{
		for (int y = -1; y < nVisibleTilesY + 1; y++)
		{
			if (World::GetWorld()->IsBlock(x + fOffsetX, y + fOffsetY))
			{
				//pge->Draw(olc::vi2d(x, y), olc::BLACK);
				//pge->DrawRect(olc::vi2d(x * 16, y * 16), olc::vi2d(16, 16), olc::BLACK);
				pge->FillRect(olc::vi2d(x * tileSize - fTileOffsetX, y * tileSize - fTileOffsetY), olc::vi2d(tileSize, tileSize), olc::BLACK);
			}
			else
			{
				//pge->Draw(olc::vi2d(x, y), olc::WHITE);
				//pge->DrawRect(olc::vi2d(x * 16, y * 16), olc::vi2d(16, 16), olc::WHITE);
				pge->FillRect(olc::vi2d(x * tileSize - fTileOffsetX, y * tileSize - fTileOffsetY), olc::vi2d(tileSize, tileSize), olc::WHITE);
			}
		}
	}

	if (pge->IsFocused())
	{
		if (pge->GetKey(olc::Key::W).bHeld)
		{
			fCameraPosY -= 10.0f * pge->GetElapsedTime();
		}
		if (pge->GetKey(olc::Key::A).bHeld)
		{
			fCameraPosX -= 10.0f * pge->GetElapsedTime();
		}
		if (pge->GetKey(olc::Key::D).bHeld)
		{
			fCameraPosX += 10.0f * pge->GetElapsedTime();
		}
		if (pge->GetKey(olc::Key::S).bHeld)
		{
			fCameraPosY += 10.0f * pge->GetElapsedTime();
		}
	}

}
void Renderer::SetCamera(olc::PixelGameEngine* pge)
{
	 nVisibleTilesX = pge->ScreenWidth() / tileSize;
	 nVisibleTilesY = pge->ScreenHeight() / tileSize;
}

