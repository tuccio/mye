#include "GameObject.h"

#include "Components.h"
#include "Game.h"

#include <algorithm>


using namespace mye::core;
using namespace std;

MYE_DEFINE_POOL_ALLOCATOR(GameObject)

GameObject::GameObject(void) :
	m_owner(nullptr),
	m_behaviour(nullptr)
{
	TransformComponent t;
	AddComponent(dynamic_cast<const Component &>(t));
}

GameObject::GameObject(const String & name) :
	INamedObject(name),
	m_owner(nullptr),
	m_behaviour(nullptr)
{
	TransformComponent t;
	AddComponent(dynamic_cast<const Component&>(t));
}


GameObject::~GameObject(void)
{
	Clear();
}

/* Components */

Component * GameObject::AddComponent(const Component & component)
{

	Component * result;

	auto it = m_components.find(component.GetName());

	if (it != m_components.end() && it->second != nullptr)
	{
		result = nullptr;
	}
	else
	{

		ComponentTypes componentType = component.GetComponentType();

		if (componentType == ComponentTypes::TRANSFORM)
		{
			m_transform = static_cast<const TransformComponent &>(component);
			m_components[m_transform.GetName()] = &m_transform;
			result = static_cast<Component*>(&m_transform);
		}
		else
		{

			Component * newComponent = static_cast<Component *>(component.Clone());
			m_components[component.GetName()] = newComponent;

			newComponent->OnAttach(this);

			switch (componentType)
			{

			case ComponentTypes::BEHAVIOUR:
				m_behaviour = static_cast<BehaviourComponent *>(newComponent);
				break;

			}

			MYE_EVENT_MANAGER_TRIGGER(GameObjectEventAddComponent, this, newComponent);

			result = newComponent;

		}

	}

	return result;

}

Component * GameObject::AddComponent(Component * component)
{

	Component * result;

	auto it = m_components.find(component->GetName());

	if (it != m_components.end() && it->second != nullptr)
	{

		if (component->GetComponentType() == ComponentTypes::TRANSFORM)
		{

			auto from = m_transform.GetWorldMatrix();

			m_transform = * static_cast<const TransformComponent *>(component);
			m_transform.OnAttach(this);
			result = &m_transform;

			MYE_EVENT_MANAGER_ENQUEUE(GameObjectEventMove, this, from, m_transform.GetWorldMatrix());

			delete component;
		}
		else
		{
			result = nullptr;
		}

	}
	else
	{

		ComponentTypes componentType = component->GetComponentType();

		Component * newComponent = component;
		m_components[newComponent->GetName()] = newComponent;

		if (newComponent->GetOwner())
		{
			newComponent->OnDetach();
		}

		newComponent->OnAttach(this);

		switch (componentType)
		{

		case ComponentTypes::BEHAVIOUR:
			m_behaviour = static_cast<BehaviourComponent *>(newComponent);
			break;

		}

		MYE_EVENT_MANAGER_TRIGGER(GameObjectEventAddComponent, this, newComponent);

		result = newComponent;

	}

	return result;

}

Component * GameObject::GetComponent(const String & name)
{

	auto it = m_components.find(name);

	if (it != m_components.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}

}

void GameObject::RemoveComponent(const String & name)
{

	auto it = m_components.find(name);

	if (it != m_components.end())
	{

		switch (it->second->GetComponentType())
		{

		case ComponentTypes::TRANSFORM:
			assert("Cannot remove TransformComponent from a GameObject");
			break;

		case ComponentTypes::BEHAVIOUR:
			m_behaviour = nullptr;
			m_entity.Clear();
			break;
		}

		MYE_EVENT_MANAGER_TRIGGER(GameObjectEventRemoveComponent, this, it->second);

		it->second->OnDetach();

		delete it->second;

		m_components.erase(it);

	}

}
void GameObject::Clear(void)
{

	/*int n = m_components.size();

	std::vector<ComponentTypes> componentTypes(n);
	std::vector<String> componentNames(n);

	for (int i = 0; i < m_components.size(); i++)
	{
		componentNames[i] = m_components[i]->GetName();
		componentTypes[i] = m_components[i]->GetComponentType();
	}

	for (int i = 0; i < n; i++)
	{

		if (componentTypes[i] != ComponentTypes::TRANSFORM)
		{
			RemoveComponent(componentNames[i]);
		}

	}*/

	std::vector<const String *> components;

	for (auto & p : m_components)
	{

		if (p.second->GetComponentType() != ComponentTypes::TRANSFORM)
		{
			components.push_back(&(p.first));
		}

	}

	for (auto c : components)
	{
		RemoveComponent(*c);
	}

	m_owner = nullptr;

	m_entity.Clear();
	m_name.Clear();

}

void GameObject::OnCreation(GameObjectsManager     * owner,
                            const GameObjectHandle & handle)
{

	m_owner    = owner;
	m_handle   = handle;

	m_deleteFlag = false;

	Game::GetSingleton().GetScriptModule()->Init(this);

	MYE_EVENT_MANAGER_ENQUEUE_NEXT_FRAME(GameObjectEventCreate, m_handle);

}

void GameObject::OnDestruction(void)
{

	Game::GetSingleton().GetScriptModule()->Finalize(this);

	Clear();

}