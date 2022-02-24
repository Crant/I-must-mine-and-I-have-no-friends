#pragma once

#include "olcPixelGameEngine.h"
#include "GameObject.h"
#include "Observer.h"

class Player : public GameObject, public Observed
{
public:
	Player(uint32_t iD, const std::string& playerName, 
		const std::string& texturePath, const olc::vf2d& position,
		const olc::vf2d& scale);
	
	void UpdateDirection(const olc::vf2d& direction);
	
	// Inherited via WorldObject
	virtual void Update(olc::PixelGameEngine* pge, float dt) override;

	virtual void Render(olc::PixelGameEngine* pge) override;

	void RenderLocal(olc::PixelGameEngine* pge, float screenWidth, float screenHeight);

	const olc::vf2d& GetDirection() const { return mDirection; }
	void SetDirection(const olc::vf2d& direction) { mDirection = direction; }
private:
	olc::vf2d mVelocity;
	olc::vf2d mDirection;
	olc::vf2d mScale;
};