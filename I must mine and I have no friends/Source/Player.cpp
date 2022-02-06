#include "Player.h"
#include "../Maths.h"
#include "Globals.h"
#include "AssetsManager.h"
#include "World.h"
#include "GameEngine.h"

using namespace IMM;

PhysObj::PhysObj(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel)
{
	vPos = vInitPos;
	this->vSize = vSize;
	vVel = { 0.f, 0.f };
	this->sModel = sModel;

	GameEngine::cObjects.push_back(this);
}
PhysObj::~PhysObj()
{

}
PhysObj::PhysObj()
{
	vPos = GlobalState::GetScreenMid();
	vVel = { 0.f, 0.f };
	this->sModel = SpriteType::Default;
}
void PhysObj::DrawSelf()
{
	GlobalState::GetEngine()->DrawPartialDecal(
		GlobalState::WorldToScreen(vPos),
		Assets::Main()->GetSpriteDecal(sModel),
		olc::vi2d(0, 0),
		olc::vi2d(vSize.x * GlobalState::GetPixelSize(), vSize.y * GlobalState::GetPixelSize()),
		olc::vf2d(GlobalState::GetTileScale(), GlobalState::GetTileScale()));
}
void PhysObj::HandleCollision()
{


	//Bygger en rektangel från din position till den positionen du kan hamna på-
	//Nästa frame och kollar varje tile inom denna rektangel för collision
	if (!CollisionCheck())
	{
		//vVel.y += 30.f * GlobalState::GetEngine()->GetElapsedTime();
	}
	vPos += vVel * GlobalState::GetEngine()->GetElapsedTime();
}
bool PhysObj::CollisionCheck()
{
	if (vVel.x == 0 && vVel.y == 0)
	{
		return false;
	}

	olc::vf2d vMaxPos;
	vMaxPos.x = (vPos.x  + (vVel.x * GlobalState::GetEngine()->GetElapsedTime())); // UTAN SIZE + ANNARS
	vMaxPos.y = (vPos.y + (vVel.y * GlobalState::GetEngine()->GetElapsedTime()));

	olc::vf2d vMinPos = vPos;

	if (vMinPos.x > vMaxPos.x)
	{
		std::swap(vMinPos.x, vMaxPos.x);
	}
	if (vMinPos.y > vMaxPos.y)
	{
		std::swap(vMinPos.y, vMaxPos.y);
	}
	vMaxPos += vSize;
	bool bHasCollided = false; 
	std::vector<CollisionData> vAllCol;

	for (float x = vMinPos.x; x < vMaxPos.x + 1; x++)
	{
		for (float y = vMinPos.y; y < vMaxPos.y + 1; y++)
		{

			if (World::Main()->IsBlock(x, y))
			{
				World::Main()->FlagBlock(x, y);
				CollisionData col;
				if (RayVsTile(olc::vf2d((int)x, (int)y), col, vMinPos))
				{
					if (col.fTime <= 1.0f)
					{
						vAllCol.push_back(col);
						bHasCollided = true;
					}
				}
			}
		}
	}
	std::sort(vAllCol.begin(), vAllCol.end(), [](const CollisionData& first, const CollisionData& second)
		{
			return first.fTime < second.fTime;
		});

	if (bHasCollided)
	{
		for (auto& col : vAllCol)
		{
			vVel += (col.vContactNormal * olc::vf2d(std::abs(vVel.x), std::abs(vVel.y)) * (1 - col.fTime));
		}
		//vVel += (vAllCol.front().vContactNormal * olc::vf2d(std::abs(vVel.x), std::abs(vVel.y)) * (1 - vAllCol.front().fTime));
		return true;
	}
	return false;
}
bool PhysObj::RayVsTile(const olc::vf2d& target, CollisionData& col, const olc::vf2d& vMinPos)
{
	olc::vf2d ray_direction = vVel * GlobalState::GetEngine()->GetElapsedTime();
	olc::vf2d vStart = vPos + vSize / 2;
	olc::vf2d vTargetPos = target - vSize / 2;
	olc::vf2d vTargetSize = vSize + olc::vi2d(1, 1);
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