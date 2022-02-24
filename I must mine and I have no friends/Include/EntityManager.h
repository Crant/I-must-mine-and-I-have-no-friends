#pragma once
//#include "olcPixelGameEngine.h"
//#include "PhysicsObject.h"
//#include "Inventory.h"
#include "Storage.h"

#include <deque>

using namespace IMM;

namespace IMM
{
	class EventListener : public Observer
	{
	public:
		void OnEvent(Event*) override;
		void Init();
		std::list<Event*> mEvents;
	};
	static class EntityManager 
	{
	public:
		EntityManager();
		~EntityManager();

		static std::shared_ptr<Storage> GetStorageAtPos(const olc::vf2d& vWorldPos);
		static void UpdatePhysObjs();
		//static void UpdateEvents();
		static void AddObj(std::shared_ptr<Updated> objPtr);
		static void RemoveObj(std::shared_ptr<Updated> objPtr);
		static int AmountOfObjects();
		//static inline EventListener events;

	private:
		static inline std::list<std::shared_ptr<Updated>> mObjects;
	};

}

