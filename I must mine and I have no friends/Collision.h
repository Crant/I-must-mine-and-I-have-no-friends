#pragma once
#include "Include/olcPixelGameEngine.h"

namespace Col
{


	olc::vf2d RayVsWorld(const olc::vf2d& vRayStart, const olc::vf2d& vRayDir, float fMaxDistance);
}


struct rect
{
	olc::vf2d pos;
	olc::vf2d size;
};

class Collision
{
public:
	Collision();
	~Collision();

	static bool PointVsRect(const olc::vf2d& point, const rect& rect);
	static bool RectVsRect(const rect& rect1, const rect& rect2);
	static bool RayVsRect(const olc::vf2d& ray_origin, const olc::vf2d& ray_direction, const rect& target, olc::vf2d& contact_point, olc::vf2d& contact_normal, float& nearCol);
private:

};