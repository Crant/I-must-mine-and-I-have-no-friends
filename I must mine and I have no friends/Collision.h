#pragma once
#include "Include/olcPixelGameEngine.h"
#include "Include/PhysicsObject.h"

using namespace IMM;

namespace Col
{
	olc::vf2d RayVsWorld(const olc::vf2d& vRayStart, const olc::vf2d& vRayDir, float fMaxDistance);
	bool RayVsTile(const olc::vf2d& target, PhysObj* obj, CollisionData& col);
}
