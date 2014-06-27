#pragma once

#include "Module.h"
#include "Camera.h"
#include "GameObject.h"

#include <list>

namespace mye
{

	namespace core
	{

		struct SceneCameraListener
		{
			virtual void OnCameraChange(Camera *oldCamera, Camera *newCamera) = 0;
		};

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
				const mye::math::AABB &oldAABB);

			inline Camera& Camera(void);

			inline mye::core::Camera* GetCamera(void);
			inline const mye::core::Camera* GetCamera(void) const;
			inline void SetCamera(mye::core::Camera *camera);

			void AddCameraListener(SceneCameraListener *listener);
			void RemoveCameraListener(SceneCameraListener *listener);

		protected:

			struct GameObjectUpdate
			{
				GameObjectHandle hObj;
				mye::math::AABB oldAABB;
				mye::math::AABB newAABB;
			};

			mye::core::Camera *m_camera;
			std::list<GameObjectUpdate> m_movedObjects;

			std::vector<SceneCameraListener*> m_cameraListeners;

		};

	}

}

#include "SceneModule.inl"