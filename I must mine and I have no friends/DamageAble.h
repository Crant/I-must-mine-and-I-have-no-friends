#pragma once
#include "Include/olcPixelGameEngine.h"
class DamageAble
{
public:
	static bool TryFindAt(olc::vf2d pos, DamageAble& target);
};

