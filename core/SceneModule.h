#pragma once

#include "Module.h"
#include "Camera.h"
#include "GameObject.h"
#include "EventManager.h"

#include <list>

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		typedef Camera CameraType;

		typedef std::list<GameObject*> GameObjectsList;
		typedef std::list<Light*>      LightsList;

		struct SceneCameraListener
		{
			virtual void OnCameraChange(Camera * oldCamera, Camera * newCamera) = 0;
		};

		struct GameObjectRayIntersection
		{
			GameObjectHandle hObj;
			mye::math::Real t;
		};

		class SceneModule :
			public Module,
			public IEventListener,
			public Singleton<SceneModule>
		{

		public:

			typedef std::list<Text2DComponent*> Text2DList;

			SceneModule(void);

			bool Init(void);
			void Shutdown(void);

			virtual GameObjectsList GetVisibleObjects(const mye::math::Frustum & frustum) = 0;
			virtual GameObjectsList GetVisibleObjects(const mye::core::Camera & camera) = 0;

			virtual GameObjectsList GetVisibleLights(const mye::math::Frustum & frustum) = 0;
			virtual GameObjectsList GetVisibleLights(const mye::core::Camera & camera) = 0;

			virtual GameObjectsList GetObjectsList(void);

			virtual GameObjectsList FindObjects(const mye::math::AABB & aabb) = 0;

			const LightsList & GetLightsList(void) const;
			const Text2DList & GetText2DList(void) const;

			virtual void AddGameObject    (const GameObjectHandle & hObj);
			virtual void RemoveGameObject (const GameObjectHandle & hObj);

			virtual GameObjectRayIntersection Pick(const mye::math::Ray & ray);
			
			inline void MoveGameObject(const GameObjectHandle & hObj, const mye::math::AABB & oldAABB);

			inline CameraType & Camera(void);

			inline CameraType       * GetCamera(void);
			inline const CameraType * GetCamera(void) const;
			inline void               SetCamera(CameraType * camera);

			void AddCameraListener    (SceneCameraListener * listener);
			void RemoveCameraListener (SceneCameraListener * listener);

			void OnComponentAddition (GameObject * go, Component * component);
			void OnComponentRemoval  (GameObject * go, Component * component);

			void OnEvent(const IEvent * e);

			virtual mye::math::AABB GetAABB(void) = 0;

		protected:

			struct GameObjectUpdate
			{
				GameObjectHandle hObj;
				mye::math::AABB  oldAABB;
				mye::math::AABB  newAABB;
			};

			CameraType * m_camera;

			std::list<GameObjectUpdate> m_movedObjects;

			std::vector<SceneCameraListener*> m_cameraListeners;

			LightsList m_lights;
			Text2DList m_textes;

		};

	}

}

#include "SceneModule.inl"