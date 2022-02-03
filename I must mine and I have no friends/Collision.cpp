#include "Collision.h"
#include "Include/World.h"

using namespace IMM;


Collision::Collision()
{
}

Collision::~Collision()
{
}
bool Collision::PointVsRect(const olc::vf2d& point, const rect& rect)
{
	return (point.x >= rect.pos.x && point.y >= rect.pos.y && point.x >= rect.pos.x + rect.size.x && point.y >= rect.pos.y + rect.size.y);
}
bool Collision::RectVsRect(const rect& rect1, const rect& rect2)
{
	return (rect1.pos.x < rect2.pos.x + rect2.size.x && rect1.pos.x + rect1.size.x > rect2.pos.x &&
		rect1.pos.y < rect2.pos.y + rect2.size.y && rect1.pos.y + rect1.size.y > rect2.pos.y);
}
bool Collision::RayVsRect(const olc::vf2d& ray_origin, const olc::vf2d& ray_direction, const rect& target, olc::vf2d& contact_point, olc::vf2d& contact_normal, float& time)
{
	// 

	olc::vf2d nearIntersect = (target.pos - ray_origin) / ray_direction;
	olc::vf2d farIntersect = (target.pos + target.size - ray_origin) / ray_direction;

	if (nearIntersect.x > farIntersect.x)
	{
		std::swap(nearIntersect.x, farIntersect.x);
	}
	if (nearIntersect.y > farIntersect.y)
	{
		std::swap(nearIntersect.y, farIntersect.y);
	}

	if (nearIntersect.x > farIntersect.y || nearIntersect.y > farIntersect.x)
	{
		return false;
	}

	time = std::max(nearIntersect.x, nearIntersect.y);
	float farCol = std::min(farIntersect.x, farIntersect.y);

	if (farCol < 0)
	{
		return false;
	}

	contact_point = ray_origin + time * ray_direction;

	if (nearIntersect.x > nearIntersect.y)
	{
		if (ray_direction.x < 0)
		{
			contact_normal = { 1, 0 };
		}
		else
		{
			contact_normal = { -1, 0 };
		}
	}
	else if (nearIntersect.x < nearIntersect.y)
	{
		if (ray_direction.y < 0)
		{
			contact_normal = { 0, 1 };
		}
		else
		{
			contact_normal = { 0, -1 };
		}
	}
	return true;
}

olc::vf2d Col::RayVsWorld(const olc::vf2d& vRayStart, const olc::vf2d& vRayDir, float fMaxDistance)
{
	olc::vf2d vStepSize = { sqrt(1 + (vRayDir.y / vRayDir.x) * (vRayDir.y / vRayDir.x)), sqrt(1 + (vRayDir.x / vRayDir.y) * (vRayDir.x / vRayDir.y)) };

	olc::vi2d vTileCheck = vRayStart;
	olc::vf2d vRayLength1D;

	olc::vi2d vStep;

	if (vRayDir.x < 0)
	{
		vStep.x = -1;
		vRayLength1D.x = (vRayStart.x - float(vTileCheck.x)) * vStepSize.x;
	}
	else
	{
		vStep.x = 1;
		vRayLength1D.x = (float(vTileCheck.x + 1) - vRayStart.x) * vStepSize.x;
	}

	if (vRayDir.y < 0)
	{
		vStep.y = -1;
		vRayLength1D.y = (vRayStart.y - float(vTileCheck.y)) * vStepSize.y;
	}
	else
	{
		vStep.y = 1;
		vRayLength1D.y = (float(vTileCheck.y + 1) - vRayStart.y) * vStepSize.y;
	}

	bool bTileFound = false;
	float fDistance = 0.0f;
	while (!bTileFound && fDistance < fMaxDistance)
	{
		if (vRayLength1D.x < vRayLength1D.y)
		{
			vTileCheck.x += vStep.x;
			fDistance = vRayLength1D.x;
			vRayLength1D.x += vStepSize.x;
		}
		else
		{
			vTileCheck.y += vStep.y;
			fDistance = vRayLength1D.y;
			vRayLength1D.y += vStepSize.y;
		}
		if (World::Main()->IsBlock(vTileCheck))
		{
			bTileFound = true;
		}
	}
	olc::vf2d vIntersection = olc::vf2d(0.f, 0.f);

	if (bTileFound)
	{
		vIntersection = vRayStart + vRayDir * fDistance;
	}
	//else
	//{
	//	*vIntersection = olc::vf2d(0.f, 0.f);
	//}
	return vIntersection;
}