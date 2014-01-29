#include "GameObjectsManager.h"
#include "GameObject.h"

using namespace mye::core;

GameObjectHandle::GameObjectHandle(void)
{
	id = -1;
	allocation = -1;
}

GameObjectHandle::GameObjectHandle(int id, int allocation)
{
	this->id = id;
	this->allocation = allocation;
}

GameObjectsManager::GameObjectsManager(void) :
	m_lastId(-1)
{

	if (!PoolAllocator<GameObject>::GetSingletonPointer())
	{
		new PoolAllocator<GameObject>();
	}

}


GameObjectsManager::~GameObjectsManager(void)
{
}

GameObjectHandle GameObjectsManager::Create(void)
{

	return Create(std::string());

}

GameObjectHandle GameObjectsManager::Create(const std::string& name)
{

	GameObjectHandle hObj = CreateHandle();

	GameObject *o = new GameObject(name);
	m_objects[hObj] = o;

	if (!name.empty())
	{
		m_namedObjects.insert(std::pair<std::string, GameObjectHandle>(name, hObj));
	}

	o->OnCreation(this, hObj);

	return hObj;

}

// void GameObjectsManager::Rename(const GameObjectHandle &hObj,
// 			const std::string &name)
// {
// 	throw;
// }

void GameObjectsManager::Destroy(const GameObjectHandle &hObj)
{

	GameObject *o = Get(hObj);

	if (o)
	{

		std::string name = o->GetName();

		if (!name.empty())
		{

			for (auto eqr = m_namedObjects.equal_range(name);
				eqr.first != eqr.second;
				eqr.first++)
			{

				if (eqr.first->second == hObj)
				{
					m_namedObjects.erase(eqr.first);
					break;
				}

			}

		}

		delete o;
		m_objects.erase(m_objects.find(hObj));
		FreeHandle(hObj);

	}

}

GameObjectHandle GameObjectsManager::Find(const std::string &name)
{

	auto eqr = m_namedObjects.equal_range(name);

	if (eqr.first != m_namedObjects.end())
	{
		return eqr.first->second;
	}

	return GameObjectHandle();

}