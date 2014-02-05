#pragma once

#include "Module.h"
#include "Camera.h"
#include "GameObject.h"

#include <list>

namespace mye
{

	namespace core
	{

		class SceneModule :
			public Module
		{

		public:

			typedef std::list<GameObject*> ObjectsList;

			SceneModule(void);

			virtual ObjectsList GetVisibleObjects(void);

			virtual void AddGameObject(const GameObjectHandle &hObj);
			virtual void RemoveGameObject(const GameObjectHandle &hObj);

			inline mye::core::Camera& Camera(void);

			inline mye::core::Camera* GetCamera(void);
			inline const mye::core::Camera* GetCamera(void) const;
			inline const void SetCamera(mye::core::Camera *camera);

		protected:

			mye::core::Camera *m_camera;

		};

	}

}

#include "SceneModule.inl"