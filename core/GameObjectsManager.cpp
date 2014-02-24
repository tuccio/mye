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
}


GameObjectsManager::~GameObjectsManager(void)
{
}

GameObjectHandle GameObjectsManager::Create(void)
{

	return Create(String());

}

GameObjectHandle GameObjectsManager::Create(const String& name)
{

	GameObjectHandle hObj = CreateHandle();

	GameObject *o = new GameObject(name);
	m_objects[hObj] = o;

	if (!name.IsEmpty())
	{
		m_namedObjects.insert(std::pair<String, GameObjectHandle>(name, hObj));
	}

	o->OnCreation(this, hObj);

	return hObj;

}

// void GameObjectsManager::Rename(const GameObjectHandle &hObj,
// 			const String &name)
// {
// 	throw;
// }

void GameObjectsManager::Destroy(const GameObjectHandle &hObj)
{

	GameObject *o = Get(hObj);

	if (o)
	{

		String name = o->GetName();

		if (!name.IsEmpty())
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

GameObjectHandle GameObjectsManager::Find(const String &name)
{

	auto eqr = m_namedObjects.equal_range(name);

	if (eqr.first != m_namedObjects.end())
	{
		return eqr.first->second;
	}

	return GameObjectHandle();

}

void GameObjectsManager::Rename(const GameObjectHandle &hObj, const String &name)
{

	GameObject *o = Get(hObj);

	if (o)
	{

		const String &oldName = o->GetName();

		if (!oldName.IsEmpty())
		{

			for (auto eqr = m_namedObjects.equal_range(oldName);
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

		o->SetName(name);

		if (!name.IsEmpty())
		{
			m_namedObjects.insert(std::pair<String, GameObjectHandle>(name, hObj));
		}

	}

	

}