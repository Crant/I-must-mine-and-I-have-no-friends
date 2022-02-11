#include "Collision.h"
#include "Include/World.h"
#include "Include/Globals.h"

using namespace IMM;

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
	return vIntersection;
}
bool Col::RayVsTile(const olc::vf2d& target, IMM::PhysObj* obj, IMM::CollisionData& col)
{
	olc::vf2d ray_direction = obj->vVel * GlobalState::GetEngine()->GetElapsedTime();
	olc::vf2d vStart = obj->vPos + obj->vSize / 2;
	olc::vf2d vTargetPos = target - obj->vSize / 2;
	olc::vf2d vTargetSize = obj->vSize + olc::vi2d(1, 1);
	olc::vf2d nearIntersect = (vTargetPos - vStart) / ray_direction;
	olc::vf2d farIntersect = (vTargetPos + vTargetSize - vStart) / ray_direction;

	if (std::isnan(farIntersect.y) || std::isnan(farIntersect.x))
	{
		return false;
	}
	if (std::isnan(nearIntersect.y) || std::isnan(nearIntersect.x))
	{
		return false;
	}
	olc::vf2d contact_point;
	olc::vf2d contact_normal;

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

	float fTime = std::max(nearIntersect.x, nearIntersect.y);
	float farCol = std::min(farIntersect.x, farIntersect.y);

	if (farCol < 0 || fTime < 0)
	{
		return false;
	}

	contact_point = vStart + fTime * ray_direction;

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
	col.fTime = fTime;
	col.vContactNormal = contact_normal;
	col.vContactPoint = contact_point;
	return true;
}
