#include "EntityManager.h"
#include "Globals.h"
#include "World.h"

std::shared_ptr<Storage> EntityManager::GetStorageAtPos(const olc::vf2d& vWorldPos)
{
	for (auto& obj : mObjects)
	{
		if (obj->tType == ObjectType::Storage)
		{
			auto cStorage = std::reinterpret_pointer_cast<Storage>(obj);

			//if ((int)cStorage->vPos.x == (int)vWorldPos.x && (int)cStorage->vPos.y == (int)vWorldPos.y)
			//{
			//	return cStorage;
			//}
			//else if ((int)cStorage->vPos.x == (int)vWorldPos.x - 1 && (int)cStorage->vPos.y == (int)vWorldPos.y)
			//{
			//	return cStorage;
			//}
			//else if ((int)cStorage->vPos.x == (int)vWorldPos.x && (int)cStorage->vPos.y == (int)vWorldPos.y - 1)
			//{
			//	return cStorage;
			//}
			//else if ((int)cStorage->vPos.x == (int)vWorldPos.x + 1 && (int)cStorage->vPos.y == (int)vWorldPos.y)
			//{
			//	return cStorage;
			//}
			//else if ((int)cStorage->vPos.x == (int)vWorldPos.x && (int)cStorage->vPos.y == (int)vWorldPos.y + 1)
			//{
			//	return cStorage;
			//}
			//else if ((int)cStorage->vPos.x == (int)vWorldPos.x + 1 && (int)cStorage->vPos.y == (int)vWorldPos.y + 1)
			//{
			//	return cStorage;
			//}
			//else if ((int)cStorage->vPos.x == (int)vWorldPos.x - 1 && (int)cStorage->vPos.y == (int)vWorldPos.y - 1)
			//{
			//	return cStorage;
			//}
			if (cStorage->vPos.x <= vWorldPos.x &&
				vWorldPos.x <= cStorage->vPos.x + cStorage->vSize.x &&
				cStorage->vPos.y <= vWorldPos.y &&
				vWorldPos.y <= cStorage->vPos.y + cStorage->vSize.y)
			{
				return cStorage;
			}
			
		}
	}
	return nullptr;
}
void EntityManager::UpdatePhysObjs()
{
	auto it = mObjects.begin();

	while (it != mObjects.end())
	{
		if (!(*it)->UpdateSelf())
		{
			mObjects.erase(it++);
		}
		else
		{
			++it;
		}
	}
}
//void IMM::EntityManager::UpdateEvents()
//{
//	auto it = events.mEvents.begin();
//
//	while (it != events.mEvents.end())
//	{
//		for (auto& obj : mObjects)
//		{
//			if (obj->ListenToEvents(*it))
//			{
//				events.mEvents.erase(it++);
//			}
//			else
//			{
//				++it;
//			}
//		}
//	}
//
//
//
//
//
//
//
//
//
//	for (auto& e : events.mEvents)
//	{
//
//	}
//}
void EntityManager::AddObj(std::shared_ptr<Updated> objPtr) // ANTAGLIGEN KOMMER INTE ANVÄNDAS 
{
	mObjects.push_back(objPtr);
}

void EntityManager::RemoveObj(std::shared_ptr<Updated> objPtr)// ANTAGLIGEN KOMMER INTE ANVÄNDAS 
{
	mObjects.remove(objPtr);
}
int EntityManager::AmountOfObjects()
{
	return mObjects.size();
}

void IMM::EventListener::OnEvent(Event* e)
{
	//if (e->type == EventType::TileRemoved)
	//{
	//	auto TRE = reinterpret_cast<IMM::Events::TileRemovedEvent*>(e);
	//	mEvents.push_back(TRE);
	//}
	//mEvents.push_back(e);
}

void IMM::EventListener::Init()
{
	//World::Main()->AddObserver(this);
}
