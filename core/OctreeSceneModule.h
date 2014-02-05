#pragma once

#include "SceneModule.h"
#include "GameObject.h"

#include <list>

namespace mye
{

	namespace core
	{

		class OctreeSceneModule :
			public SceneModule
		{

		public:

			OctreeSceneModule(void);
			~OctreeSceneModule(void);

			SceneModule::ObjectsList GetVisibleObjects(void);

			void AddGameObject(const GameObjectHandle &hObj);
			void RemoveGameObject(const GameObjectHandle &hObj);

		private:

			std::list<GameObjectHandle> m_objects;

		};

	}

}
