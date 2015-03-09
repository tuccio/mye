#include "GameObjectsManager.h"
#include "GameObject.h"
#include "Entity.h"
#include "ResourceTypeManager.h"
#include "Utils.h"

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

GameObjectHandle GameObjectsManager::Create(const String & name)
{

	__ObjectInfo objectInfo = __InstantiateObject(name);
	__InitObject(objectInfo);

	return objectInfo.handle;

}

GameObjectHandle GameObjectsManager::CreateEntity(const String & entity)
{

	return CreateEntity(entity, String());

}

GameObjectHandle GameObjectsManager::CreateEntity(const String & entity,
												  const String & name)
{

	__ObjectInfo objectInfo;

	EntityPointer e = ResourceTypeManager::GetSingleton().
		CreateResource<Entity>("Entity", entity);

	Parameters params;
	params["type"] = "behaviour";

	mye::core::ScriptPointer s = ResourceTypeManager::GetSingleton().
		CreateResource<Script>("Script", entity, nullptr, params);

	if (e && e->Load())
	{

		if (s && s->Load())
		{

			objectInfo = __InstantiateObject(name);

			objectInfo.object->m_entity = entity;

			if (!e->Instantiate(objectInfo.object))
			{
				FreeHandle(objectInfo.handle);
				delete objectInfo.object;
				objectInfo = { GameObjectHandle(), nullptr };
			}
			else
			{

				objectInfo.object->AddComponent(new BehaviourComponent(s));
				__InitObject(objectInfo);

			}

		}
		else
		{

			String error("Cannot load entity script ");
			error += s->GetName();

			RuntimeError(error);

		}

	}
	else
	{

		String error("Cannot load entity declaration ");
		error += e->GetName();

		RuntimeError(error);

	}

	return objectInfo.handle;

}

// void GameObjectsManager::Rename(const GameObjectHandle &hObj,
// 			const String &name)
// {
// 	throw;
// }

void GameObjectsManager::Destroy(const GameObjectHandle & hObj)
{

	GameObject *o = Get(hObj);

	if (o)
	{

		o->OnDestruction();

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

GameObjectHandle GameObjectsManager::Find(const String & name)
{

	auto eqr = m_namedObjects.equal_range(name);

	if (eqr.first != m_namedObjects.end())
	{
		return eqr.first->second;
	}

	return GameObjectHandle();

}

void GameObjectsManager::Rename(const GameObjectHandle & hObj, const String & name)
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

GameObjectsManager::Iterator GameObjectsManager::begin(void)
{
	return Iterator(this, &m_activeObjects, m_activeObjects.begin());
}

GameObjectsManager::Iterator GameObjectsManager::end(void)
{
	return Iterator(this, &m_activeObjects, m_activeObjects.end());
}

ActiveGameObjectsIterator::ActiveGameObjectsIterator(GameObjectsManager * gom,
													 std::list<GameObjectHandle> * list,
													 const std::list<GameObjectHandle>::iterator & it) :
	m_it(it),
	m_list(list),
	m_gom(gom)
{


	while (m_it != m_list->end() && m_gom->Get(*m_it) == nullptr)
	{
		m_it++;
	};

}

ActiveGameObjectsIterator & ActiveGameObjectsIterator::operator++ (void)
{
	
	do 
	{
		m_it++;
	} while (m_it != m_list->end() && m_gom->Get(*m_it) == nullptr);

	return *this;

}

ActiveGameObjectsIterator ActiveGameObjectsIterator::operator++ (int)
{

	ActiveGameObjectsIterator it = *this;

	++(*this);

	return it;

}

bool ActiveGameObjectsIterator::operator != (const ActiveGameObjectsIterator & it) const
{
	return it.m_it != m_it;
}

GameObjectHandle ActiveGameObjectsIterator::operator* (void) const
{
	return *m_it;
}

GameObjectsManager::__ObjectInfo GameObjectsManager::__InstantiateObject(const String & name)
{

	__ObjectInfo objectInfo;

	objectInfo.handle = CreateHandle();
	objectInfo.object = new GameObject(name);

	m_objects[objectInfo.handle] = objectInfo.object;

	return objectInfo;

}

void GameObjectsManager::__InitObject(__ObjectInfo objectInfo)
{

	if (!objectInfo.object->m_name.IsEmpty())
	{
		m_namedObjects.insert(std::pair<String, GameObjectHandle>(objectInfo.object->m_name, objectInfo.handle));
	}

	objectInfo.object->OnCreation(this, objectInfo.handle);

	m_activeObjects.push_front(objectInfo.handle);

}