#include "EntityManager.h"
#include "Globals.h"

std::shared_ptr<Entity> EntityManager::GetEntityAtPos(const olc::vf2d& vWorldPos)
{
	//for (auto& obj : mObjects)
	//{

	//	if ((int)obj->vPos.x == (int)vWorldPos.x && (int)obj->vPos.y == (int)vWorldPos.y)
	//	{
	//		return obj;
	//	}
	//	else if ((int)obj->vPos.x == (int)vWorldPos.x - 1 && (int)obj->vPos.y == (int)vWorldPos.y)
	//	{
	//		return obj;
	//	}
	//	else if ((int)obj->vPos.x == (int)vWorldPos.x && (int)obj->vPos.y == (int)vWorldPos.y - 1)
	//	{
	//		return obj;
	//	}
	//	else if ((int)obj->vPos.x == (int)vWorldPos.x + 1 && (int)obj->vPos.y == (int)vWorldPos.y)
	//	{
	//		return obj;
	//	}
	//	else if ((int)obj->vPos.x == (int)vWorldPos.x && (int)obj->vPos.y == (int)vWorldPos.y + 1)
	//	{
	//		return obj;
	//	}
	//	else if ((int)obj->vPos.x == (int)vWorldPos.x + 1 && (int)obj->vPos.y == (int)vWorldPos.y + 1)
	//	{
	//		return obj;
	//	}
	//	else if ((int)obj->vPos.x == (int)vWorldPos.x - 1 && (int)obj->vPos.y == (int)vWorldPos.y - 1)
	//	{
	//		return obj;
	//	}
	//}
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

	//for (auto& obj : mObjects)
	//{
	//	obj->UpdateSelf();
	//}
	//GlobalState::GetEngine()->DrawStringDecal(GlobalState::GetEngine()->GetMousePos(), std::to_string(mObjects.size()));
}
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
