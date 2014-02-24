#include "GameObject.h"

#include "ScriptComponent.h"
#include "TransformComponent.h"

#include <mye/lua/LuaScript.h>

using namespace mye::core;
using namespace std;

MYE_DEFINE_POOL_ALLOCATOR(GameObject)

GameObject::GameObject(void) :
	m_owner(nullptr),
	m_transform(nullptr),
	m_script(nullptr),
	m_render(nullptr),
	m_camera(nullptr)
{
}

GameObject::GameObject(const String &name) :
	INamedObject(name),
	m_owner(nullptr),
	m_transform(nullptr),
	m_script(nullptr),
	m_render(nullptr),
	m_camera(nullptr)
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

	if (it != m_components.end() && it->second != nullptr)
	{
		return nullptr;
	}
	else
	{

		Component *newComponent = static_cast<Component*>(component.Clone());
		m_components[component.GetName()] = newComponent;

		newComponent->m_owner = this;

		switch (component.GetComponentType())
		{

		case ComponentTypes::TRANSFORM:
			m_transform = static_cast<TransformComponent*>(newComponent);
			break;

		case ComponentTypes::SCRIPT:
			m_script = static_cast<ScriptComponent*>(newComponent);
			break;

		case ComponentTypes::RENDER:
			m_render = static_cast<RenderComponent*>(newComponent);
			break;

		case ComponentTypes::CAMERA:
			m_camera = static_cast<CameraComponent*>(newComponent);
			break;

		}

		return newComponent;

	}

}

Component* GameObject::GetComponent(const String &name)
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

void GameObject::RemoveComponent(const String &name)
{

	auto it = m_components.find(name);

	if (it != m_components.end())
	{

		switch (it->second->GetComponentType())
		{

		case ComponentTypes::TRANSFORM:
			m_transform = nullptr;
			break;

		case ComponentTypes::SCRIPT:
			m_script = nullptr;
			break;

		case ComponentTypes::RENDER:
			m_render = nullptr;
			break;

		case ComponentTypes::CAMERA:
			m_camera = nullptr;
			break;

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

	m_owner     = nullptr;

	m_transform = nullptr;
	m_script    = nullptr;

}

void GameObject::OnCreation(GameObjectsManager *owner,
							const GameObjectHandle &handle)
{

	m_owner = owner;
	m_handle = handle;

	TransformComponent *t = static_cast<TransformComponent*>(AddComponent(TransformComponent()));

}

void GameObject::OnDestruction(void)
{

	Clear();

}

void GameObject::Update(FloatSeconds dt)
{

	if (m_script)
	{
		m_script->Script().Call<void, float>("Update", dt);
	}

}