#include "GameObject.h"

using namespace mye::core;
using namespace std;

GameObject::GameObject(void)
{
	_parent = NULL;
}

GameObject::GameObject(const std::string &name) :
	_name(name)
{
	_parent = NULL;
}


GameObject::~GameObject(void)
{
}

/* Components */

void GameObject::AddComponent(const std::string &name,
							  const Component &component)
{

	auto it = _components.find(name);

	Component *newComponent = static_cast<Component*>(component.Clone());

	if (it != _components.end() && it->second != NULL)
	{
		delete it->second;
		it->second = newComponent;
	}
	else
	{
		_components[name] = newComponent;
	}

}

Component* GameObject::GetComponent(const std::string &name)
{

	auto it = _components.find(name);

	if (it != _components.end())
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

	auto it = _components.find(name);

	if (it != _components.end())
	{
		_components.erase(it);
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
	_components.clear();
	_children.clear();
	_parent = NULL;
}

/* Parents and children */

GameObject* GameObject::GetParent(void)
{
	return _parent;
}

void GameObject::SetParent(GameObject *parent)
{

	// If the object has a parent, we remove this object
	// from the old parent children

	if (_parent)
	{

		auto it = std::find(_parent->_children.begin(),
			_parent->_children.end(),
			this);

		if (it != _parent->_children.end())
		{
			_parent->_children.erase(it);
		}

	}

	// If the new parent is not null, we add our object to his children
	// If it's null, this object will be orphan

	if (parent)
	{
		parent->_children.push_back(this);
	}

	_parent = parent;

}

const GameObject::ChildrenList& GameObject::GetChildren(void) const
{
	return _children;
}

void GameObject::OnCreation(GameObjectsManager *owner,
							const GameObjectHandle &handle)
{
	_owner = owner;
	_handle = handle;
}

void GameObject::OnDestruction(void)
{

	if (_parent)
	{
		_parent->OnChildDestruction(this);
	}

	for (auto child : _children)
	{
		child->OnParentDestruction();
	}

	Clear();

}

void GameObject::OnParentDestruction(void)
{
	_parent = NULL;
}

void GameObject::OnChildDestruction(GameObject *child)
{
	child->SetParent(NULL);
}

GameObjectsManager* GameObject::GetOwner(void)
{
	return _owner;
}

GameObjectHandle GameObject::GetHandle(void)
{
	return _handle;
}

void GameObject::SetOwner(GameObjectsManager *owner)
{
	_owner = owner;
}

void GameObject::SetHandle(const GameObjectHandle &handle)
{
	_handle = handle;
}