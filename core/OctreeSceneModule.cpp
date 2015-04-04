#include "OctreeSceneModule.h"
#include "Game.h"

#include <deque>

using namespace mye::core;
using namespace mye::math;

OctreeSceneModule::OctreeSceneModule(const mye::math::Vector3 &center,
									 float size,
									 unsigned int maxdepth,
									 unsigned int looseness)
//	m_octree(center, size, maxdepth, looseness)
{
}


OctreeSceneModule::~OctreeSceneModule(void)
{
}

#ifdef __MYE_NO_OCTREE

GameObjectsList OctreeSceneModule::GetVisibleObjects(const Matrix4 & viewProjection)
{

	GameObjectsList list;

	for (GameObjectHandle hObj : m_objects)
	{
		list.push_back(Game::GetSingleton().GetGameObjectsModule()->Get(hObj));
	}

	return list;

}

void OctreeSceneModule::AddGameObject(const GameObjectHandle &hObj)
{

	SceneModule::AddGameObject(hObj);

	m_objects.push_back(hObj);

}

void OctreeSceneModule::RemoveGameObject(const GameObjectHandle &hObj)
{

	SceneModule::RemoveGameObject(hObj);

	m_objects.erase(std::find(m_objects.begin(), m_objects.end(), hObj));

}

GameObjectRayIntersection OctreeSceneModule::Pick(const mye::math::Ray & ray)
{

	GameObjectsList objects;

	mye::math::Real tMin = std::numeric_limits<mye::math::Real>::infinity();
	GameObjectHandle rObj;

	for (GameObjectHandle hObj : m_objects)
	{

		GameObject * object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

		if (object)
		{
			
			RenderComponent *rc = object->GetRenderComponent();
			mye::math::Real t;

			if (rc && Intersect(ray, rc->GetBounds().
				TransformAffine(object->GetTransformComponent()->GetWorldMatrix()), t) && t < tMin)
			{
				tMin = t;
				rObj = object->GetHandle();
			}

		}

	}

	return { rObj, tMin };

}

void OctreeSceneModule::ApplyUpdates(void)
{

}

void OctreeSceneModule::Reset(const mye::math::Vector3 & center,
							  float size,
							  unsigned int maxdepth,
							  unsigned int looseness)
{

	m_objects.clear();

}

GameObjectsList OctreeSceneModule::GetObjectsList(void)
{

	GameObjectsList objects;

	for (GameObjectHandle hObj : m_objects)
	{

		GameObject * object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

		if (object)
		{
			objects.push_back(object);
		}

	}

	return objects;

}

#else

SceneModule::ObjectsList OctreeSceneModule::GetVisibleObjects(void)
{

	ApplyUpdates();	

	SceneModule::ObjectsList visibleObjects;

	Frustum frustum = m_camera->GetFrustum();
	AABB frustumAABB = BoundingAABB(frustum);

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
				AABB aabb = object->GetAABB();

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

SceneModule::ObjectsList OctreeSceneModule::GetObjects(void)
{

	SceneModule::ObjectsList objects;

	std::deque<LooseOctreeNode<GameObjectHandle>*> q;

	q.push_back(m_octree.GetRoot());

	do 
	{

		LooseOctreeNode<GameObjectHandle> *node = q.front();
		q.pop_front();

		if (node)
		{

			for (GameObjectHandle hObj : *node)
			{

				GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

				if (object)
				{
					objects.push_back(object);
				}

			}

			for (int i = 0; i < 8; i++)
			{
				q.push_back(node->GetChild(static_cast<OctreeChild>(i)));
			}

		}

	} while (!q.empty());

	for (auto it = m_nonRenderableObjects.begin(); it != m_nonRenderableObjects.end(); it++)
	{

		GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(*it);

		if (object)
		{
			objects.push_back(object);
		}
		else
		{
			it = m_nonRenderableObjects.erase(it);
		}

	}

	return objects;

}

void OctreeSceneModule::AddGameObject(const GameObjectHandle &hObj)
{

	GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

	if (object->GetRenderComponent())
	{
		m_octree.Insert(hObj, object->GetAABB());
	}
	else
	{
		m_nonRenderableObjects.push_back(hObj);
	}
	
}

void OctreeSceneModule::RemoveGameObject(const GameObjectHandle &hObj)
{

	GameObject *object = Game::GetSingleton().GetGameObjectsModule()->Get(hObj);

	if (object->GetRenderComponent())
	{
		m_octree.Remove(hObj, object->GetAABB());
	}
	else
	{
		
		auto it = std::find(m_nonRenderableObjects.begin(), m_nonRenderableObjects.end(), hObj);

		if (it != m_nonRenderableObjects.end())
		{
			m_nonRenderableObjects.erase(it);
		}

	}
	
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

void OctreeSceneModule::Reset(
	const mye::math::Vector3 &center,
	float size,
	unsigned int maxdepth,
	unsigned int looseness)
{

	m_octree = LooseOctree<GameObjectHandle>(center, size, maxdepth, looseness);

}

#endif