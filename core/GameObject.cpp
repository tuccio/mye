#include "GameObject.h"

#include "ScriptComponent.h"
#include "TransformComponent.h"

#include <mye/lua/LuaScript.h>

using namespace mye::core;
using namespace std;

GameObject::GameObject(void) :
	m_owner(NULL),
	m_transform(NULL),
	m_script(NULL)
{
}

GameObject::GameObject(const std::string &name) :
	m_name(name),
	m_owner(NULL),
	m_transform(NULL),
	m_script(NULL)
{
}


GameObject::~GameObject(void)
{
	Clear();
}

/* Components */

Component* GameObject::AddComponent(const Component &component)
{

	auto it = m_components.find(component.GetName());

	if (it != m_components.end() && it->second != NULL)
	{
		return NULL;
	}
	else
	{

		Component *newComponent = static_cast<Component*>(component.Clone());
		m_components[component.GetName()] = newComponent;

		switch (component.GetComponentType())
		{
		case COMPONENT_TRANSFORM:
			m_transform = static_cast<TransformComponent*>(newComponent);
			break;
		case COMPONENT_SCRIPT:
			m_script = static_cast<ScriptComponent*>(newComponent);
		}

		return newComponent;

	}

}

Component* GameObject::GetComponent(const std::string &name)
{

	auto it = m_components.find(name);

	if (it != m_components.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}

}

void GameObject::RemoveComponent(const std::string &name)
{

	auto it = m_components.find(name);

	if (it != m_components.end())
	{

		switch (it->second->GetComponentType())
		{
		case COMPONENT_TRANSFORM:
			m_transform = NULL;
			break;
		case COMPONENT_SCRIPT:
			m_script = NULL;
		}

		delete it->second;
		m_components.erase(it);

	}

}
void GameObject::Clear(void)
{

	for (auto it : m_components)
	{
		delete it.second;
	}

	m_components.clear();

	m_owner     = NULL;

	m_transform = NULL;
	m_script    = NULL;

}

// void* GameObject::operator new (std::size_t size)
// {
// 	return PoolAllocator<GameObject>::GetSingleton().Allocate();
// }
// 
// void GameObject::operator delete (void *p)
// {
// 	PoolAllocator<GameObject>::GetSingleton().Free(p);
// }

void GameObject::OnCreation(GameObjectsManager *owner,
							const GameObjectHandle &handle)
{

	m_owner = owner;
	m_handle = handle;

	TransformComponent *t = static_cast<TransformComponent*>(AddComponent(TransformComponent()));
	t->Set(mye::math::Transformf::Identity());

}

void GameObject::OnDestruction(void)
{

	Clear();

}

GameObjectsManager* GameObject::GetOwner(void)
{
	return m_owner;
}

GameObjectHandle GameObject::GetHandle(void)
{
	return m_handle;
}

void GameObject::Update(FloatSeconds dt)
{

	if (m_script)
	{
		m_script->Script().Call<void, float>("Update", dt);
	}

}