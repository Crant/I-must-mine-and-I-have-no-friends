#pragma once

#include "Include/olcPixelGameEngine.h"
#include "Renderer.h"
#include "ActiveItem.h"
class Player
{
	Renderer clientRenderer;
	ActiveItem activeItem;
	olc::PixelGameEngine* pge;
public:
	void UpdatePlayer();//KALLAS VARJE FRAME
};

