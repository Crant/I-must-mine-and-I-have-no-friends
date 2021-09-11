#pragma once

#include "olcPixelGameEngine.h"

class DamageAble
{
public:
	static bool TryFindAt(olc::vf2d pos, DamageAble& target);
};

