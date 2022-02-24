#include "PhysicsObject.h"
#include "../Maths.h"
#include "Globals.h"
#include "AssetsManager.h"
#include "World.h"
//#include "GameEngine.h"
#include "GraphicsRenderer.h"
#include "../Collision.h"
#include "EntityManager.h"

using namespace IMM;

PhysObj::PhysObj(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel)
	: Entity(vInitPos, vSize, sModel), vVel(0.f, 0.f), bIsGrounded(false)
{

}
PhysObj::~PhysObj()
{
	//GameEngine::cObjects.remove(this);
}
void PhysObj::ApplyForce(const olc::vf2d& vPush)
{
	vVel += vPush; //Använd när man blir träffad av saker osv
}
bool PhysObj::UpdateSelf()
{

	vVel.y += World::Main()->GetGravity() * GlobalState::GetEngine()->GetElapsedTime();
	CollisionCheck();
	vPos += vVel * GlobalState::GetEngine()->GetElapsedTime();
	World::Main()->CheckWrapping(vPos.x, vPos.x);
	if (fabs(vVel.x) < 0.01f)
	{
		vVel.x = 0.0f;
	}
	Entity::UpdateSelf();
	return true;
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
	bIsGrounded = false;
	std::vector<CollisionData> vAllCol;

	for (float x = vMinPos.x; x < vMaxPos.x + 1; x++)
	{
		for (float y = vMinPos.y; y < vMaxPos.y + 1; y++)
		{

			if (World::Main()->IsBlock(x, y))
			{
				World::Main()->FlagBlock(x, y);
				CollisionData col; //Här ligger all data för kollisioner och används senare för att hantera kollisionerna
				if (Col::RayVsTile(olc::vf2d((int)x, (int)y), this, col))
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
	if (bHasCollided)
	{
		ResolveCollisions(vAllCol);
		return true;
	}
	else
	{
		return false;
	}
	//std::sort(vAllCol.begin(), vAllCol.end(), [](const CollisionData& first, const CollisionData& second)
	//{
	//	return first.fTime < second.fTime;
	//});

	//if (bHasCollided)
	//{
	//	for (auto& col : vAllCol)
	//	{


	//		vVel += (col.vContactNormal * olc::vf2d(std::abs(vVel.x), std::abs(vVel.y)) * (1 - col.fTime));
	//		if (col.vContactNormal == olc::vf2d(0, -1))
	//		{
	//			bIsGrounded = true;
	//		}
	//	}
	//	return true;
	//}
	//return false;
}
void PhysObj::ResolveCollisions(std::vector<CollisionData>& vAllCol)
{
	std::sort(vAllCol.begin(), vAllCol.end(), [](const CollisionData& first, const CollisionData& second)
	{
		return first.fTime < second.fTime;
	});

	for (auto& col : vAllCol)
	{


		vVel += (col.vContactNormal * olc::vf2d(std::abs(vVel.x), std::abs(vVel.y)) * (1 - col.fTime));
		if (col.vContactNormal == olc::vf2d(0, -1))
		{
			bIsGrounded = true;
		}
	}
}
//////////////////////////////////////////////////////////
//PhysUnit
PhysUnit::PhysUnit(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, float fHitpoints)
	: PhysObj(vInitPos, vSize, sModel)
{
	this->fHitpoints = fHitpoints;
	Updated::tType = ObjectType::Unit;
}
bool PhysUnit::UpdateSelf()
{
	MoveUnit();
	PhysObj::UpdateSelf();
	return true;
}
void PhysUnit::MoveUnit()
{
	//Lägg in kod för AI osv här eller annat sätt unit rör sig
}
/// <summary>
/// ///////////////
/// </summary>
/// <param name="vInitPos"></param>
/// <param name="vSize"></param>
/// <param name="sModel"></param>
/// <param name="vVel"></param>
/// 
PhysProjectile::PhysProjectile(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, const olc::vf2d& vVel, float fLifeTimer, float fDmg)
	: PhysObj(vInitPos, vSize, sModel)
{
	this->vVel = vVel;
	this->fLifeTimer = fLifeTimer;
	this->fDmg = fDmg;
}
PhysProjectile::PhysProjectile(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, const olc::vf2d& vVel, float fDmg)
	:PhysObj(vInitPos, vSize, sModel), fLifeTimer(-100.f)
{
	this->vVel = vVel;
	this->fDmg = fDmg;
}

bool PhysProjectile::UpdateSelf()
{
	vVel.y += World::Main()->GetGravity() * GlobalState::GetEngine()->GetElapsedTime();
	if (CollisionCheck() && fLifeTimer <= 0.f)
	{
		return false;
	}
	vPos += vVel * GlobalState::GetEngine()->GetElapsedTime();
	World::Main()->CheckWrapping(vPos.x, vPos.x);
	if (fabs(vVel.x) < 0.01f)
	{
		vVel.x = 0.0f;
	}
	fLifeTimer -= GlobalState::GetEngine()->GetElapsedTime();
	Entity::UpdateSelf();
	return true;
}
void PhysProjectile::ResolveCollisions(std::vector<CollisionData>& vAllCol)
{
	std::sort(vAllCol.begin(), vAllCol.end(), [](const CollisionData& first, const CollisionData& second)
		{
			return first.fTime < second.fTime;
		});

	for (auto& col : vAllCol)
	{
		vVel += (col.vContactNormal * olc::vf2d(std::abs(vVel.x), std::abs(vVel.y)) * (1 - col.fTime));
		World::Main()->DamageBlock(col.vContactPoint - col.vContactNormal, fDmg);
		if (col.vContactNormal == olc::vf2d(0, -1))
		{
			bIsGrounded = true;
		}
	}
}