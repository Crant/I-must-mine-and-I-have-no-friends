#include "TilePhysics.h"

using namespace IMM;

void TilePhysics::UpdatePhysics(olc::PixelGameEngine* pge)
{
	fTotalTime += pge->GetElapsedTime();
	if (fTotalTime < 0.02f)
	{
		return;
	}
	for (size_t x = 0; x < World::Main()->GetWidth(); x++)
	{
		for (size_t y = 0; y < World::Main()->GetHeight(); y++)
		{
			if (World::Main()->IsBlock(x, y) && !(World::Main()->IsBlock(x, y - 1)))
			{
				World::Main()->SetTile(x, y - 1, *World::Main()->GetTile(x, y));
				World::Main()->SetTile(x, y, TileType::Empty);
			}
		}
	}
	//UpdateNbours();
	fTotalTime = 0.0f;
}
void TilePhysics::UpdateNbours()
{
	for (size_t x = 0; x < World::Main()->GetWidth(); x++)
	{
		for (size_t y = 0; y < World::Main()->GetHeight(); y++)
		{
			if (World::Main()->IsBlock(x, y))
			{
				World::Main()->SetNeighbours(x, y);
			}
		}
	}
}
//void TilePhysics::UpdatePhysics(olc::PixelGameEngine* pge)
//{
//	//fTotalTime += pge->GetElapsedTime();
//	//if (fTotalTime < 0.02f)
//	//{
//	//	return;
//	//}
//	//for (size_t x = 0; x < World::Main()->GetWidth(); x++)
//	//{
//	//	for (size_t y = 0; y < World::Main()->GetHeight(); y++)
//	//	{
//	//		if (World::Main()->IsBlock(x, y) && !(World::Main()->IsBlock(x, y - 1)))
//	//		{
//
//	//		}
//	//	}
//	//}
//	//for (size_t x = 0; x < World::Main()->GetWidth(); x++)
//	//{
//	//	for (size_t y = 0; y < World::Main()->GetHeight(); y++)
//	//	{
//	//		World::Main()->SetNeighbours(x, y);	
//	//	}
//	//}
//}