#include "../Include/Game.h"

bool Game::OnUserCreate()
{
	// Called once at the start, so create things here

	Assets::get().LoadSprites();
	GridGenerator gridGen("REEE", worldWidth, worldHeight, this);
	Tiles::LoadTiles();
	
	renderer.SetCamera();
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	// called once per frame
	Clear(olc::DARK_BLUE);

	renderer.UpdateCamera();
	OnUserFixedUpdate();

	if (GetKey(olc::Key::ENTER).bHeld)
		return false;

	return true;
}

void Game::OnUserFixedUpdate()
{
	fTimer += GetElapsedTime();
	if (fTimer > fFixedUpdate)
	{
		TileController::CheckActiveTiles();
		//physX.UpdatePhysics(this);
		fTimer = 0.0f;
	}
}