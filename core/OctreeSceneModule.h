#pragma once

#include "SceneModule.h"
#include "GameObject.h"

#include <list>

#include <mye/math/Math.h>

#define __MYE_NO_OCTREE

namespace mye
{

	namespace core
	{

		class OctreeSceneModule :
			public SceneModule
		{

		public:

			OctreeSceneModule(const mye::math::Vector3 & center = mye::math::Vector3(0),
							  float size = 1024.0f,
							  unsigned int maxdepth = 32,
							  unsigned int looseness = 2);

			~OctreeSceneModule(void);

			GameObjectsList GetVisibleObjects(const mye::math::Matrix4 & viewProjection);

			GameObjectsList GetObjectsList(void);

			void AddGameObject(const GameObjectHandle & hObj);
			void RemoveGameObject(const GameObjectHandle & hObj);

			GameObjectRayIntersection Pick(const mye::math::Ray & ray);

			void Reset(const mye::math::Vector3 &center = mye::math::Vector3(0),
					   float size = 1024.0f,
					   unsigned int maxdepth = 32,
					   unsigned int looseness = 2);

		private:

			void ApplyUpdates(void);

			std::list<GameObjectHandle>   m_objects;
			std::vector<GameObjectHandle> m_nonRenderableObjects;

		};

	}

}