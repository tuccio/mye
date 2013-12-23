#include "GameObject.h"

#include "NullComponent.h"

using namespace mye::core;
using namespace std;

GameObject::GameObject(void)
{
}


GameObject::~GameObject(void)
{
}

/* Components */

void GameObject::AddComponent(const Component &component)
{
	
	if (!component.IsNull())
	{
		const string &name = component.GetName();
		_components[name] = static_cast<Component*>(component.Clone());
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

void GameObject::SetType(const std::string &type)
{
	_type = type;
}

const std::string& GameObject::GetType(void) const
{
	return _type;
}

void GameObject::Destroy(void)
{
}

void GameObject::Clear(void)
{
	_components.clear();
}