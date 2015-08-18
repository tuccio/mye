#include "OctreeSceneModule.h"
#include "Game.h"

using namespace mye::core;
using namespace mye::math;
using namespace mye::algorithms;

OctreeSceneModule::OctreeSceneModule(const Vector3 & center,
									 float size,
									 unsigned int maxdepth) :
									 m_dynamicObjects(center, size, maxdepth),
									 m_lights(center, size, maxdepth, detail::LightAABBBounds { { center.x(), center.y(), center.z() }, size * .5f })
{
}

OctreeSceneModule::~OctreeSceneModule(void)
{
}

GameObjectsList OctreeSceneModule::GetVisibleObjects(const mye::math::Frustum & frustum)
{

	GameObjectsList list;

	m_dynamicObjects.Query(frustum, [&list] (GameObject * object) { list.push_back(object); });

	return list;

}

GameObjectsList OctreeSceneModule::GetVisibleObjects(const mye::core::Camera & camera)
{
	return GetVisibleObjects(camera.GetFrustum());
}

GameObjectsList OctreeSceneModule::GetVisibleLights(const mye::math::Frustum & frustum)
{

	GameObjectsList list;

	m_lights.Query(frustum, [&list] (GameObject * object) { list.push_back(object); });

	return list;

}

GameObjectsList OctreeSceneModule::GetVisibleLights(const mye::core::Camera & camera)
{
	return GetVisibleLights(camera.GetFrustum());
}

GameObjectsList OctreeSceneModule::GetObjectsList(void)
{
	return m_objects;
}

void OctreeSceneModule::AddGameObject(GameObject * gameObject)
{

	if (gameObject)
	{

		SceneModule::AddGameObject(gameObject);

		if (gameObject->GetRenderComponent() && m_dynamicObjects.Insert(gameObject))
		{
			m_objects.push_back(gameObject);
		}

		if (gameObject->GetLightComponent())
		{
			m_lights.Insert(gameObject);
		}

	}

}

void OctreeSceneModule::RemoveGameObject(GameObject * gameObject)
{

	if (gameObject)
	{

		if (gameObject->GetRenderComponent())
		{
			m_dynamicObjects.Remove(gameObject);
		}

		if (gameObject->GetLightComponent())
		{
			m_lights.Remove(gameObject);
		}

		SceneModule::RemoveGameObject(gameObject);

	}
	
}

GameObjectRayIntersection OctreeSceneModule::Pick(const Ray & ray)
{

	Real t;
	GameObject * object;

	if (m_dynamicObjects.Pick(ray, object, t))
	{
		return { object->GetHandle(), t };
	}

	else
	{
		return { GameObjectHandle(), -.0f };
	}

}

void OctreeSceneModule::Reset(const mye::math::Vector3 & center,
							  float size,
							  unsigned int maxdepth)
{

	m_dynamicObjects.Destroy();
	m_lights.Destroy();

	m_dynamicObjects.Create(center, size, maxdepth);
	m_lights.Create(center, size, maxdepth, detail::LightAABBBounds { { center.x(), center.y(), center.z() }, size * .5f });

}

AABB OctreeSceneModule::GetAABB(void)
{
	return m_dynamicObjects.GetAABB();
}

GameObjectsList OctreeSceneModule::FindObjects(const AABB & aabb)
{

	GameObjectsList list;

	m_dynamicObjects.Query(aabb, [&list] (GameObject * object) { list.push_back(object); });

	return list;

}