#pragma once
#include "Include/olcPixelGameEngine.h"
#include "DamageAble.h"
class DamageObj
{
	void CheckDamage();
	void CalculateDamage(olc::vf2d pos, DamageAble& target);
};

