#pragma once

//#include "olcPixelGameEngine.h"
#include "Entity.h"

using namespace IMM;

namespace IMM 
{
	struct CollisionData
	{
		olc::vf2d vContactPoint;
		olc::vf2d vContactNormal;
		float fTime;

		CollisionData()
		{
			vContactPoint = { 0.f, 0.f };
			vContactNormal = { 0.f, 0.f };
			fTime = 0.f;
		}
	};
	class PhysObj : public Entity
	{
	public:
		PhysObj(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel);
		~PhysObj();

		virtual void ApplyForce(const olc::vf2d& vPush);
		bool UpdateSelf() override;

		//DATA
		olc::vf2d vVel;
		bool bIsGrounded;
		//DATA
	protected:
		bool CollisionCheck();
		virtual void ResolveCollisions(std::vector<CollisionData>& vAllCol);
	private:


	protected:
		olc::vf2d vTempPush; //En pre-velocity variabel som clearas varje frame

	};

	/// <summary>
	/// //////////////////////////////
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// PHYSICS UNIT
	/// 
	/// 
	/// 
	/// 
	/// </summary>
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	class PhysUnit : public PhysObj
	{
	public:
		PhysUnit(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModelfloat, float fHitpoints);
		bool UpdateSelf() override;
		olc::vf2d vTarget; //Muspekaren för spelaren och för fiender etc den sak dem siktar på
	protected:

		float fHitpoints;
		virtual void MoveUnit();
		
		//void UpdateSelf() override;
	private:

	};













	/// <summary>
	/// //////////////// Projectile
	/// </summary>
	class PhysProjectile : public PhysObj
	{
	public:
		PhysProjectile(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, const olc::vf2d& vVel, float fLifeTimer, float fDmg);
		PhysProjectile(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel, const olc::vf2d& vVel, float fDmg);


		bool UpdateSelf() override;

	protected:
		void ResolveCollisions(std::vector<CollisionData>& vAllCol) override;

	private:
		float fLifeTimer;
		float fDmg;
	};

}



