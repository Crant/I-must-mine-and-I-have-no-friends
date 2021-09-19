#pragma once

#include "olcPixelGameEngine.h"

class GameObject
{
public:
	GameObject(const std::string& imgPath, const olc::vf2d& pos) :
		mPosition(pos), 
		mDecal(std::make_unique<olc::Decal>(new olc::Sprite(imgPath))) {}

	virtual ~GameObject() {  }

	virtual void Update(olc::PixelGameEngine* pge, float dt) = 0;
	virtual void Render(olc::PixelGameEngine* pge) = 0;

	virtual const int GetWidth() const { return mDecal->sprite->width; }
	virtual const int GetHeight() const{ return mDecal->sprite->height; }
	virtual const olc::vf2d& GetPosition() const { return mPosition; }
	virtual void SetPosition(const olc::vf2d& position) { mPosition = position; }

protected:
	olc::vf2d mPosition;
	std::unique_ptr<olc::Decal> mDecal;
};