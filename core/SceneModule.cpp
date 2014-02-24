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