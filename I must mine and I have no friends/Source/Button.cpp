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
		mTint = olc::DARK_GREY;
		return true;
	}

	mTint = olc::GREY;
	return false;
}

bool Button::Pressed(const float mouseX, const float mouseY)
{
	//float width = mButtonImg->sprite->width * mScale.x;
	//float height = mButtonImg->sprite->height * mScale.y;

	//if (mPosition.x <= mouseX &&
	//	mouseX <= mPosition.x + width &&
	//	mPosition.y <= mouseY &&
	//	mouseY <= mPosition.y + height)
	//{
	//	mTint = olc::DARK_GREY;
	//	return true;
	//}

	//mTint = olc::GREY;
	return false;
}

void Button::Update(olc::PixelGameEngine* pge, float dt)
{
}

void Button::Render(olc::PixelGameEngine* pge, float offsetX = 0.0f, float offsetY = 0.0f)
{
	pge->DrawDecal(mPosition, mButtonImg, mScale, mTint);
}

void Button::UpdatePos(const olc::vf2d& vNewPos)
{
	mPosition = vNewPos;
}

void Button::UpdateScale(const olc::vf2d& vNewScale)
{
	mScale = vNewScale;
}
