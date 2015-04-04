#pragma once

#include "Module.h"
#include "Camera.h"
#include "GameObject.h"

#include <list>

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
			public GameObjectListener
		{

		public:

			typedef std::list<Text2DComponent*> Text2DList;

			SceneModule(void);

			GameObjectsList GetVisibleObjects(void);
			GameObjectsList GetVisibleObjects(CameraType * camera);

			virtual GameObjectsList GetVisibleObjects(const mye::math::Matrix4 & viewProjection);

			virtual GameObjectsList GetObjectsList(void);

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