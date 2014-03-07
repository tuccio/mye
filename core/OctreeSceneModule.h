#pragma once

#include "SceneModule.h"
#include "GameObject.h"
#include "LooseOctree.h"

#include <list>

namespace mye
{

	namespace core
	{

		class OctreeSceneModule :
			public SceneModule
		{

		public:

			OctreeSceneModule(
				const mye::math::Vector3f &center = mye::math::Vector3f(0),
				float size = 1024.0f,
				unsigned int maxdepth = 32,
				unsigned int looseness = 2);

			~OctreeSceneModule(void);

			SceneModule::ObjectsList GetVisibleObjects(void);
			SceneModule::ObjectsList GetObjects(void);

			void AddGameObject(const GameObjectHandle &hObj);
			void RemoveGameObject(const GameObjectHandle &hObj);

			void Reset(
				const mye::math::Vector3f &center = mye::math::Vector3f(0),
				float size = 1024.0f,
				unsigned int maxdepth = 32,
				unsigned int looseness = 2);

		private:

			void ApplyUpdates(void);

			/*std::list<GameObjectHandle> m_objects;*/

			LooseOctree<GameObjectHandle> m_octree;
			std::vector<GameObjectHandle> m_nonRenderableObjects;


		};

	}

}