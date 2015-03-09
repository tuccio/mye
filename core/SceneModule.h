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

			typedef std::list<GameObject*> ObjectsList;
			typedef std::list<LightComponent*> LightsList;
			typedef std::list<Text2DComponent*> Text2DList;

			SceneModule(void);

			virtual ObjectsList GetVisibleObjects(void);
			virtual ObjectsList GetObjectsList(void);

			const LightsList& GetLightsList(void) const;
			const Text2DList& GetText2DList(void) const;

			virtual void AddGameObject(const GameObjectHandle &hObj);
			virtual void RemoveGameObject(const GameObjectHandle &hObj);

			virtual GameObjectRayIntersection Pick(mye::math::Ray ray);
			
			inline void MoveGameObject(const GameObjectHandle &hObj, const mye::math::AABB &oldAABB);

			inline Camera& Camera(void);

			inline       mye::core::Camera * GetCamera(void);
			inline const mye::core::Camera * GetCamera(void) const;
			inline                    void   SetCamera(mye::core::Camera * camera);

			void AddCameraListener(SceneCameraListener *listener);
			void RemoveCameraListener(SceneCameraListener *listener);

			void OnComponentAddition(GameObject *go, Component *component);
			void OnComponentRemoval(GameObject *go, Component *component);

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

			LightsList m_lights;
			Text2DList m_textes;

		};

	}

}

#include "SceneModule.inl"