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
				const mye::math::Vector3f &center,
				float size,
				unsigned int maxdepth,
				unsigned int looseness);

			~OctreeSceneModule(void);

			SceneModule::ObjectsList GetVisibleObjects(void);

			void AddGameObject(const GameObjectHandle &hObj);
			void RemoveGameObject(const GameObjectHandle &hObj);

		private:

			void ApplyUpdates(void);

			/*std::list<GameObjectHandle> m_objects;*/

			LooseOctree<GameObjectHandle> m_octree;


		};

	}

}