#pragma once

#include "Module.h"
#include "CameraComponent.h"
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
			virtual ObjectsList GetObjects(void);

			virtual void AddGameObject(const GameObjectHandle &hObj);
			virtual void RemoveGameObject(const GameObjectHandle &hObj);
			
			inline void MoveGameObject(
				const GameObjectHandle &hObj,
				const mye::math::AABBf &oldAABB);

			inline CameraComponent& Camera(void);

			inline mye::core::CameraComponent* GetCamera(void);
			inline const mye::core::CameraComponent* GetCamera(void) const;
			inline void SetCamera(mye::core::CameraComponent *camera);

		protected:

			struct GameObjectUpdate
			{
				GameObjectHandle hObj;
				mye::math::AABBf oldAABB;
				mye::math::AABBf newAABB;
			};

			mye::core::CameraComponent *m_camera;
			std::list<GameObjectUpdate> m_movedObjects;

		};

	}

}

#include "SceneModule.inl"