#include "SceneModule.h"
#include "Game.h"

using namespace mye::core;
using namespace mye::math;

SceneModule::SceneModule(void)
{
	m_camera = nullptr;
}

bool SceneModule::Init(void)
{

	MYE_EVENT_MANAGER_ADD_LISTENER(this,
	                               EventType::GAME_OBJECT_CREATE,
	                               EventType::GAME_OBJECT_DESTROY,
	                               EventType::GAME_OBJECT_MOVE,
	                               EventType::GAME_OBJECT_ADD_COMPONENT,
	                               EventType::GAME_OBJECT_REMOVE_COMPONENT);

	return true;

}

void SceneModule::Shutdown(void)
{

	MYE_EVENT_MANAGER_REMOVE_LISTENER(this,
	                                  EventType::GAME_OBJECT_CREATE,
	                                  EventType::GAME_OBJECT_DESTROY,
	                                  EventType::GAME_OBJECT_MOVE,
	                                  EventType::GAME_OBJECT_ADD_COMPONENT,
	                                  EventType::GAME_OBJECT_REMOVE_COMPONENT);

}

GameObjectsList SceneModule::GetObjectsList(void)
{
	return GameObjectsList();
}

const LightsList & SceneModule::GetLightsList(void) const
{
	return m_lights;
}

const SceneModule::Text2DList & SceneModule::GetText2DList(void) const
{
	return m_textes;
}

void SceneModule::AddGameObject(const GameObjectHandle & hObj)
{
	GameObject * gameObject = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);
	AddGameObject(gameObject);
}

void SceneModule::RemoveGameObject(const GameObjectHandle & hObj)
{
	GameObject * gameObject = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);
	RemoveGameObject(gameObject);
}

void SceneModule::AddGameObject(GameObject * gameObject)
{

	assert(gameObject && "Non-existent object added to the scene");

	if (gameObject)
	{

		auto lc = gameObject->GetLightComponent();
		auto t2dc = gameObject->GetText2DComponent();

		if (lc)
		{
			m_lights.push_back(lc);
		}

		if (t2dc)
		{
			m_textes.push_back(t2dc);
		}

	}

}

void SceneModule::RemoveGameObject(GameObject * gameObject)
{

	if (gameObject)
	{

		auto lc = gameObject->GetLightComponent();
		auto t2dc = gameObject->GetText2DComponent();

		if (lc)
		{
			std::remove(m_lights.begin(), m_lights.end(), lc);
		}

		if (t2dc)
		{
			std::remove(m_textes.begin(), m_textes.end(), t2dc);
		}

	}

}

GameObjectRayIntersection SceneModule::Pick(const mye::math::Ray & ray)
{
	return { GameObjectHandle(), -1 };
}

void SceneModule::AddCameraListener(SceneCameraListener * listener)
{
	m_cameraListeners.push_back(listener);
}

void SceneModule::RemoveCameraListener(SceneCameraListener * listener)
{

	auto it = std::find(m_cameraListeners.begin(), m_cameraListeners.end(), listener);

	if (it != m_cameraListeners.end())
	{
		m_cameraListeners.erase(it);
	}

}

void SceneModule::OnComponentAddition(GameObject * go, Component * component)
{

	switch (component->GetComponentType())
	{

	case ComponentTypes::LIGHT:
		m_lights.push_back(static_cast<LightComponent*>(component));
		break;

	case ComponentTypes::TEXT2D:
		m_textes.push_back(static_cast<Text2DComponent*>(component));
		break;

	default:
		break;

	}

}

void SceneModule::OnComponentRemoval(GameObject * go, Component * component)
{

	switch (component->GetComponentType())
	{

	case ComponentTypes::LIGHT:
		std::remove(m_lights.begin(), m_lights.end(), static_cast<LightComponent*>(component));
		break;

	case ComponentTypes::TEXT2D:
		std::remove(m_textes.begin(), m_textes.end(), static_cast<Text2DComponent*>(component));
		break;

	default:
		break;

	}

}

void SceneModule::OnEvent(const IEvent * e)
{

	switch (e->event)
	{

	case EventType::GAME_OBJECT_ADD_COMPONENT:
	{
		const GameObjectEventAddComponent * goEvent = static_cast<const GameObjectEventAddComponent *>(e);
		OnComponentAddition(goEvent->object, goEvent->component);
	}
		break;

	case EventType::GAME_OBJECT_REMOVE_COMPONENT:
	{
		const GameObjectEventRemoveComponent * goEvent = static_cast<const GameObjectEventRemoveComponent *>(e);
		OnComponentRemoval(goEvent->object, goEvent->component);
	}
		break;

	case EventType::GAME_OBJECT_MOVE:
	{

		const GameObjectEventMove * goEvent = static_cast<const GameObjectEventMove *>(e);

		GameObject      * object = goEvent->object;
		RenderComponent * rc     = object->GetRenderComponent();

		if (rc)
		{
			AABB aabb = rc->GetBounds();
			MoveGameObject(object->GetHandle(), aabb.TransformAffine(goEvent->from));
		}

	}
		break;

	case EventType::GAME_OBJECT_DESTROY:
	{

		const GameObjectEventDestroy * goEvent = static_cast<const GameObjectEventDestroy *>(e);
		RemoveGameObject(goEvent->hObj);

	}
		break;

	}

}