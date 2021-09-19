#include "Player.h"
#include "PlayerEvents.h"

Player::Player(
	uint32_t iD, 
	const std::string& playerName,
	const std::string& texturePath, 
	const olc::vf2d& position,
	const olc::vf2d& scale) : GameObject(texturePath, position)
{
	mVelocity = olc::vf2d(50, 50);
	mDirection = olc::vf2d(0, 0);
	mScale = olc::vf2d(0.25f, 0.25f);
}

void Player::UpdateDirection(const olc::vf2d& direction)
{
	if (mDirection != direction)
	{
		//Normalize the direction
		if(direction.mag() > 0) 
			mDirection = direction.norm();
		else // Cant normalize a zero vector
			mDirection = direction;

		//Send Event to server
		IMM::Events::PlayerMoveEvent PME;

		PME.direction = mDirection;
		
		NotifyObservers(&PME);
	}
}

void Player::Update(olc::PixelGameEngine* pge, float dt)
{
	mPosition = mPosition + (mVelocity * mDirection * dt);
}

void Player::Render(olc::PixelGameEngine* pge)
{
	pge->DrawDecal(mPosition, mDecal.get());
}
