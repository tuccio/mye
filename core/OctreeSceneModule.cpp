#include "OctreeSceneModule.h"
#include "Game.h"

using namespace mye::core;

OctreeSceneModule::OctreeSceneModule(void)
{
}


OctreeSceneModule::~OctreeSceneModule(void)
{
}

SceneModule::ObjectsList OctreeSceneModule::GetVisibleObjects(void)
{
	
	SceneModule::ObjectsList list;

	for (GameObjectHandle hObj : m_objects)
	{
		list.push_back(Game::GetSingleton().GetGameObjectsModule()->Get(hObj));
	}

	return list;

}

void OctreeSceneModule::AddGameObject(const GameObjectHandle &hObj)
{
	m_objects.push_back(hObj);
}

void OctreeSceneModule::RemoveGameObject(const GameObjectHandle &hObj)
{
	m_objects.erase(std::find(m_objects.begin(), m_objects.end(), hObj));
}