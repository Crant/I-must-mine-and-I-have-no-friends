#pragma once
#include "olcPixelGameEngine.h"
#include "PhysicsObject.h"

#include <deque>

using namespace IMM;

namespace IMM
{
	static class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		static std::shared_ptr<Entity> GetEntityAtPos(const olc::vf2d& vWorldPos);
		static void UpdatePhysObjs();
		static void AddObj(std::shared_ptr<Updated> objPtr);
		static void RemoveObj(std::shared_ptr<Updated> objPtr);
		static int AmountOfObjects();

	private:
		static inline std::list<std::shared_ptr<Updated>> mObjects;
	};

}

