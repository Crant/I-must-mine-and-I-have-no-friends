#pragma once

#include "olcPixelGameEngine.h"
#include "GameObject.h"

class Button : public GameObject
{
public:
	Button(const std::string& imgPath, const olc::vf2d& pos, const olc::vf2d& scale, const olc::Pixel& tint = olc::WHITE);
	virtual ~Button();

	bool Hovered(const float mouseX, const float mouseY);
	bool Pressed(const float mouseX, const float mouseY);

	// Inherited via WorldObject
	virtual void Update(olc::PixelGameEngine* pge, float dt) override;
	virtual void Render(olc::PixelGameEngine* pge) override;

	int GetScaledWidth() { return mDecal->sprite->width * mScale.x; }
	int GetScaledHeight() { return mDecal->sprite->height * mScale.y; }
private:
	//std::unique_ptr<olc::Decal> mButtonImg;
	//olc::vf2d mPosition;
	olc::vf2d mScale;
	olc::Pixel mTint;
};