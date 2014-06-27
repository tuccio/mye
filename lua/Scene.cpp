#include "Scene.h"

#include <luabind/luabind.hpp>

#include <mye/core/SceneModule.h>

using namespace luabind;
using namespace mye::core;

namespace mye
{

	namespace lua
	{

		inline boost::optional<Camera*> __scene_get_camera(SceneModule &scene)
		{

			Camera *camera = scene.GetCamera();

			return (camera ? boost::optional<Camera*>(camera) : boost::optional<Camera*>());

		}

		void BindScene(lua_State *L)
		{

			module(L)
			[

				class_<SceneModule>("SceneModule").

					def("AddGameObject", &SceneModule::AddGameObject).
					def("RemoveGameObject", &SceneModule::RemoveGameObject).

					property("camera", (Camera* (SceneModule::*) ()) &SceneModule::GetCamera, &SceneModule::SetCamera)


					
				
// 					void AddGameObject(const GameObjectHandle &hObj);
// 					virtual void RemoveGameObject(const GameObjectHandle &hObj);
// 
// 					inline mye::core::CameraComponent* GetCamera(void);
// 					inline const mye::core::CameraComponent* GetCamera(void) const;
// 					inline void SetCamera(mye::core::CameraComponent *camera);


			];

		}

	}

}