#pragma once

#include "SceneModule.h"
#include "GameObject.h"

#include <list>

#define __MYE_NO_OCTREE

namespace mye
{

	namespace core
	{

		class OctreeSceneModule :
			public SceneModule
		{

		public:

			OctreeSceneModule(
				const mye::math::Vector3 &center = mye::math::Vector3(0),
				float size = 1024.0f,
				unsigned int maxdepth = 32,
				unsigned int looseness = 2);

			~OctreeSceneModule(void);

			SceneModule::ObjectsList GetVisibleObjects(void);
			SceneModule::ObjectsList GetObjectsList(void);

			void AddGameObject(const GameObjectHandle &hObj);
			void RemoveGameObject(const GameObjectHandle &hObj);

			GameObjectRayIntersection Pick(mye::math::Ray ray);

			void Reset(
				const mye::math::Vector3 &center = mye::math::Vector3(0),
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