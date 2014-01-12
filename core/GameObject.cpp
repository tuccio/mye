#include "GameObject.h"

#include "TransformComponent.h"

using namespace mye::core;
using namespace std;

GameObject::GameObject(void)
{
}

GameObject::GameObject(const std::string &name) :
	m_name(name)
{
}


GameObject::~GameObject(void)
{
}

/* Components */

Component* GameObject::AddComponent(const Component &component)
{

	auto it = m_components.find(component.GetName());

	Component *newComponent = static_cast<Component*>(component.Clone());

	if (it != m_components.end() && it->second != NULL)
	{
		return NULL;
	}
	else
	{
		m_components[component.GetName()] = newComponent;
	}

	return newComponent;

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
		delete it->second;
		m_components.erase(it);
	}

}
void GameObject::Clear(void)
{
	m_components.clear();
}

void GameObject::OnCreation(GameObjectsManager *owner,
							const GameObjectHandle &handle)
{

	m_owner = owner;
	m_handle = handle;

	TransformComponent *t = static_cast<TransformComponent*>(AddComponent(TransformComponent()));
	t->Set(Transform::Identity());

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