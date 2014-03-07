#include "SceneModule.h"

using namespace mye::core;

SceneModule::SceneModule(void)
{
	m_camera = nullptr;
}

SceneModule::ObjectsList SceneModule::GetVisibleObjects(void)
{
	return ObjectsList();
}

SceneModule::ObjectsList SceneModule::GetObjects(void)
{
	return ObjectsList();
}

void SceneModule::AddGameObject(const GameObjectHandle &hObj)
{

}

void SceneModule::RemoveGameObject(const GameObjectHandle &hObj)
{

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