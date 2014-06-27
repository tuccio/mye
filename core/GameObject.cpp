#include "GameObject.h"

#include "Components.h"
#include "Game.h"

using namespace mye::core;
using namespace std;

MYE_DEFINE_POOL_ALLOCATOR(GameObject)

GameObject::GameObject(void) :
	m_owner(nullptr),
	m_transform(nullptr),
	m_render(nullptr),
	m_camera(nullptr),
	m_behaviour(nullptr),
	m_rigidbody(nullptr)
{
}

GameObject::GameObject(const String &name) :
	INamedObject(name),
	m_owner(nullptr),
	m_transform(nullptr),
	m_render(nullptr),
	m_camera(nullptr),
	m_behaviour(nullptr),
	m_rigidbody(nullptr),
	m_text2d(nullptr)
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

		case ComponentTypes::RENDER:
			m_render = static_cast<RenderComponent*>(newComponent);
			break;

		case ComponentTypes::CAMERA:
			m_camera = static_cast<CameraComponent*>(newComponent);
			break;

		case ComponentTypes::BEHAVIOUR:
			m_behaviour = static_cast<BehaviourComponent*>(newComponent);
			break;

		case ComponentTypes::RIGIDBODY:
			m_rigidbody = static_cast<RigidBodyComponent*>(newComponent);
			Game::GetSingleton().GetPhysicsModule()->AddRigidBody(m_rigidbody->GetRigidBody());
			break;

		case ComponentTypes::TEXT2D:

			m_text2d = static_cast<Text2DComponent*>(newComponent);
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

		case ComponentTypes::RENDER:
			m_render = nullptr;
			break;

		case ComponentTypes::CAMERA:
			m_camera = nullptr;
			break;

		case ComponentTypes::BEHAVIOUR:
			m_behaviour = nullptr;
			break;

		case ComponentTypes::RIGIDBODY:
			m_rigidbody = nullptr;
			Game::GetSingleton().GetPhysicsModule()->RemoveRigidBody(m_rigidbody->GetRigidBody());
			break;

		case ComponentTypes::TEXT2D:
			m_text2d = nullptr;
			break;

		}

		delete it->second;
		m_components.erase(it);

	}

}
void GameObject::Clear(void)
{

	/*for (auto it : m_components)
	{
		delete it.second;
	}

	m_components.clear();*/

	std::vector<String> componentNames;

	for (auto p : m_components)
	{
		componentNames.push_back(p.first);
	}

	for (const String &n : componentNames)
	{
		RemoveComponent(n);
	}

	m_owner     = nullptr;

	m_entity.Clear();
	m_name.Clear();

}

void GameObject::OnCreation(GameObjectsManager *owner,
							const GameObjectHandle &handle)
{

	m_owner  = owner;
	m_handle = handle;

	m_delendum = false;

	TransformComponent *t = static_cast<TransformComponent*>(AddComponent(TransformComponent()));

	Init();

}

void GameObject::OnDestruction(void)
{

	Clear();

}

void GameObject::Init(void)
{

	if (m_behaviour)
	{
		m_behaviour->Init();
	}

}

void GameObject::Update(void)
{

	if (m_behaviour)
	{
		m_behaviour->Update();
	}

}