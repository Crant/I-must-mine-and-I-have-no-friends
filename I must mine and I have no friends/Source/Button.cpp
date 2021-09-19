#include "Button.h"
#include "Safe.h"

Button::Button(
	const std::string& imgPath, 
	const olc::vf2d& pos, 
	const olc::vf2d& scale, 
	const olc::Pixel& tint) : GameObject(imgPath, pos)
{
	mScale = scale;
	mTint = tint;
}

Button::~Button()
{
	
}

bool Button::Hovered(const float mouseX, const float mouseY)
{
	float width = mDecal->sprite->width * mScale.x;
	float height = mDecal->sprite->height * mScale.y;

	if (mPosition.x <= mouseX &&
		mouseX <= mPosition.x + width &&
		mPosition.y <= mouseY &&
		mouseY <= mPosition.y + height)
	{
		mTint = olc::GREY;
		return true;
	}

	mTint = olc::WHITE;
	return false;
}

bool Button::Pressed(const float mouseX, const float mouseY)
{
	return false;
}

void Button::Update(olc::PixelGameEngine* pge, float dt)
{
}

void Button::Render(olc::PixelGameEngine* pge)
{
	pge->DrawDecal(mPosition, mDecal.get(), mScale, mTint);
}