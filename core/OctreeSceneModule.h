#pragma once

#include "Camera.h"
#include "LightComponent.h"
#include "SceneModule.h"
#include "GameObject.h"

#include <list>

#include <mye/math/Math.h>
#include <mye/algorithms/LooseOctree.h>

namespace mye
{

	namespace core
	{

		namespace detail
		{

			struct GameObjectAABBBounds
			{

				inline mye::math::AABB operator() (GameObject * object)
				{
					return object->GetAABB();
				}

			};

			struct LightAABBBounds
			{

				inline mye::math::AABB operator() (GameObject * object)
				{
					
					using namespace mye::math;

					LightComponent * light = object->GetLightComponent();

					AABB aabb;

					switch (light->GetType())
					{

					case LightType::DIRECTIONAL:
						aabb = AABB::FromCenterHalfExtents(Vector3(worldCenter[0], worldCenter[1], worldCenter[2]), Vector3(worldHalfSize));
						break;

					case LightType::POINTLIGHT:
						aabb = AABB::FromCenterHalfExtents(light->GetPosition(), Vector3(light->GetRange()));
						break;

					case LightType::SPOTLIGHT:
						assert(false && "Not implemented yet");
						break;

					}

					return aabb;

				}

				float worldCenter[3];
				float worldHalfSize;

			};

		}

		class OctreeSceneModule :
			public SceneModule
		{

		public:

			OctreeSceneModule(const mye::math::Vector3 & center = mye::math::Vector3(0),
							  float size = 1024.0f,
							  unsigned int maxdepth = 16);

			~OctreeSceneModule(void);

			GameObjectsList GetVisibleObjects(const mye::math::Frustum & frustum);
			GameObjectsList GetVisibleObjects(const mye::core::Camera & camera);
			//GameObjectsList GetVisibleObjects(const mye::math::Matrix4 & viewProjection);

			GameObjectsList GetVisibleLights(const mye::math::Frustum & frustum);
			GameObjectsList GetVisibleLights(const mye::core::Camera & camera);

			GameObjectsList GetObjectsList(void);

			GameObjectsList FindObjects(const mye::math::AABB & aabb);

			void AddGameObject(const GameObjectHandle & hObj);
			void RemoveGameObject(const GameObjectHandle & hObj);

			GameObjectRayIntersection Pick(const mye::math::Ray & ray);

			void Reset(const mye::math::Vector3 & center = mye::math::Vector3(0),
					   float size = 1024.0f,
					   unsigned int maxdepth = 16);

			mye::math::AABB GetAABB(void);

		private:

			typedef mye::algorithms::LooseOctree<GameObject*, mye::math::AABB, detail::GameObjectAABBBounds> DynamicObjectsOctree;
			typedef mye::algorithms::LooseOctree<GameObject*, mye::math::AABB, detail::LightAABBBounds>      LightsOctree;

			GameObjectsList m_objects;

			DynamicObjectsOctree m_dynamicObjects;
			LightsOctree         m_lights;

		};

	}

}