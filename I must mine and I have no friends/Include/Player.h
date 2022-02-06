#pragma once

#include "olcPixelGameEngine.h"

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
	enum class SpriteType : unsigned char
	{
		Default
	};

	class PhysObj
	{
	public:
		PhysObj(const olc::vf2d& vInitPos, const olc::vf2d& vSize, SpriteType sModel);
		PhysObj();
		~PhysObj();

		virtual void DrawSelf();
		void HandleCollision();
		bool CollisionCheck();
		bool RayVsTile(const olc::vf2d& target, CollisionData& col, const olc::vf2d& vStart);
		//bool RayVsTile(const olc::vf2d& target, CollisionData& col);
		//bool RayVsTile(const olc::vf2d& ray_origin, const olc::vf2d& ray_direction, const olc::vf2d& target, CollisionData& col);
		//inline olc::vf2d GetPos() { return vPos; }
		olc::vf2d vPos;
		olc::vf2d vVel;
		olc::vf2d vSize;
	private:
		//bool RayVsTile(const olc::vf2d& target, CollisionData& col);

	protected:

		SpriteType sModel;
	};

	class Player : public PhysObj
	{
	public:

		using PhysObj::PhysObj;

	private:

	};
}



