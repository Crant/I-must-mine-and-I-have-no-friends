#pragma once

#include "olcPixelGameEngine.h"

class Button
{
public:
	Button();
	Button(const std::string& imgPath, const olc::vf2d& pos, const olc::vf2d& scale, const olc::Pixel& tint = olc::WHITE);
	virtual ~Button();

	bool Hovered(const float mouseX, const float mouseY);
	bool Pressed(const float mouseX, const float mouseY);

	void Render(olc::PixelGameEngine* pge);
private:
	olc::Sprite* mButtonSprite;
	olc::Decal* mButtonImg;
	olc::vf2d mPosition;
	olc::vf2d mScale;
	olc::Pixel mTint;
};