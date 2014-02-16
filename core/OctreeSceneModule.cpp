#include "OctreeSceneModule.h"
#include "Game.h"

#include <deque>

using namespace mye::core;
using namespace mye::math;

OctreeSceneModule::OctreeSceneModule(const mye::math::Vector3f &center,
									 float size,
									 unsigned int maxdepth,
									 unsigned int looseness) :
	m_octree(center, size, maxdepth, looseness)
{
}


OctreeSceneModule::~OctreeSceneModule(void)
{
}

/*
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
}*/

SceneModule::ObjectsList OctreeSceneModule::GetVisibleObjects(void)
{

	ApplyUpdates();	

	SceneModule::ObjectsList visibleObjects;

	Frustumf frustum = m_camera->GetFrustum();
	AABBf frustumAABB = BoundingAABB(frustum);

	std::deque<LooseOctreeNode<GameObjectHandle>*> q;

	q.push_back(m_octree.FindFirst(frustumAABB));

	do 
	{

		LooseOctreeNode<GameObjectHandle> *node = q.front();
		q.pop_front();

		if (node)
		{
			
			for (GameObjectHandle hObj : *node)
			{

				GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);
				AABBf aabb = object->GetAABB();

				if (Intersect(aabb, frustum) != VolumeSide::OUTSIDE)
				{
					visibleObjects.push_back(object);
				}

			}

			for (int i = 0; i < 8; i++)
			{
				q.push_back(node->GetChild(static_cast<OctreeChild>(i)));
			}

		}

	} while (!q.empty());

	return visibleObjects;

}

void OctreeSceneModule::AddGameObject(const GameObjectHandle &hObj)
{
	GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);
	m_octree.Insert(hObj, object->GetAABB());
}

void OctreeSceneModule::RemoveGameObject(const GameObjectHandle &hObj)
{
	GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);
	m_octree.Remove(hObj, object->GetAABB());
}

void OctreeSceneModule::ApplyUpdates(void)
{

	for (GameObjectUpdate update : m_movedObjects)
	{

		GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(update.hObj);

		if (object)
		{
			m_octree.Relocate(update.hObj, update.oldAABB, object->GetAABB());
		}

	}

	m_movedObjects.clear();

}