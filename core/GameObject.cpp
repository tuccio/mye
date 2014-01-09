#include "GameObject.h"

using namespace mye::core;
using namespace std;

GameObject::GameObject(void)
{
	m_parent = NULL;
}

GameObject::GameObject(const std::string &name) :
	m_name(name)
{
	m_parent = NULL;
}


GameObject::~GameObject(void)
{
}

/* Components */

void GameObject::AddComponent(const std::string &name,
							  const Component &component)
{

	auto it = m_components.find(name);

	Component *newComponent = static_cast<Component*>(component.Clone());

	if (it != m_components.end() && it->second != NULL)
	{
		delete it->second;
		it->second = newComponent;
	}
	else
	{
		m_components[name] = newComponent;
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
		m_components.erase(it);
	}

}

// void GameObject::SetName(const std::string &name)
// {
// 	_name = name;
// }
// 
// const std::string& GameObject::GetName(void) const
// {
// 	return _name;
// }

void GameObject::Clear(void)
{
	m_components.clear();
	m_children.clear();
	m_parent = NULL;
}

/* Parents and children */

GameObject* GameObject::GetParent(void)
{
	return m_parent;
}

void GameObject::SetParent(GameObject *parent)
{

	// If the object has a parent, we remove this object
	// from the old parent children

	if (m_parent)
	{

		auto it = std::find(m_parent->m_children.begin(),
			m_parent->m_children.end(),
			this);

		if (it != m_parent->m_children.end())
		{
			m_parent->m_children.erase(it);
		}

	}

	// If the new parent is not null, we add our object to his children
	// If it's null, this object will be orphan

	if (parent)
	{
		parent->m_children.push_back(this);
	}

	m_parent = parent;

}

const GameObject::ChildrenList& GameObject::GetChildren(void) const
{
	return m_children;
}

void GameObject::OnCreation(GameObjectsManager *owner,
							const GameObjectHandle &handle)
{
	m_owner = owner;
	m_handle = handle;
}

void GameObject::OnDestruction(void)
{

	if (m_parent)
	{
		m_parent->OnChildDestruction(this);
	}

	for (auto child : m_children)
	{
		child->OnParentDestruction();
	}

	Clear();

}

void GameObject::OnParentDestruction(void)
{
	m_parent = NULL;
}

void GameObject::OnChildDestruction(GameObject *child)
{
	child->SetParent(NULL);
}

GameObjectsManager* GameObject::GetOwner(void)
{
	return m_owner;
}

GameObjectHandle GameObject::GetHandle(void)
{
	return m_handle;
}

void GameObject::SetOwner(GameObjectsManager *owner)
{
	m_owner = owner;
}

void GameObject::SetHandle(const GameObjectHandle &handle)
{
	m_handle = handle;
}