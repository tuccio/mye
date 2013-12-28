#include "GameObject.h"

using namespace mye::core;
using namespace std;

GameObject::GameObject(void)
{
}

GameObject::GameObject(const std::string &name) :
	_name(name)
{
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

void GameObject::SetName(const std::string &name)
{
	_name = name;
}

const std::string& GameObject::GetName(void) const
{
	return _name;
}

void GameObject::Clear(void)
{
	_components.clear();
}