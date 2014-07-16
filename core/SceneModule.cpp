#include "SceneModule.h"
#include "Game.h"

using namespace mye::core;

SceneModule::SceneModule(void)
{
	m_camera = nullptr;
}

SceneModule::ObjectsList SceneModule::GetVisibleObjects(void)
{
	return ObjectsList();
}

SceneModule::ObjectsList SceneModule::GetObjectsList(void)
{
	return ObjectsList();
}

const SceneModule::LightsList& SceneModule::GetLightsList(void) const
{
	return m_lights;
}

const SceneModule::Text2DList& SceneModule::GetText2DList(void) const
{
	return m_textes;
}

void SceneModule::AddGameObject(const GameObjectHandle &hObj)
{

	GameObject *go = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

	assert(go && "Non-existent object added to the scene");

	if (go)
	{
		
		go->AddListener(this);

		auto lc = go->GetLightComponent();
		auto t2dc = go->GetText2DComponent();

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

void SceneModule::RemoveGameObject(const GameObjectHandle &hObj)
{

	GameObject *go = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

	if (go)
	{

		go->RemoveListener(this);

		auto lc = go->GetLightComponent();
		auto t2dc = go->GetText2DComponent();

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

GameObjectRayIntersection SceneModule::Pick(mye::math::Ray ray)
{
	return { GameObjectHandle(), -1 };
}

void SceneModule::AddCameraListener(SceneCameraListener *listener)
{
	m_cameraListeners.push_back(listener);
}

void SceneModule::RemoveCameraListener(SceneCameraListener *listener)
{

	auto it = std::find(m_cameraListeners.begin(), m_cameraListeners.end(), listener);

	if (it != m_cameraListeners.end())
	{
		m_cameraListeners.erase(it);
	}

}

void SceneModule::OnComponentAddition(GameObject *go, Component *component)
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

void SceneModule::OnComponentRemoval(GameObject *go, Component *component)
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