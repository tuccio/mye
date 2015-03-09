#include "Scene.h"

#include <mye/core/SceneModule.h>

using namespace luapp11;
using namespace mye::core;

namespace mye
{

	namespace lua
	{

		/*inline boost::optional<Camera*> __scene_get_camera(SceneModule &scene)
		{

			Camera *camera = scene.GetCamera();

			return (camera ? boost::optional<Camera*>(camera) : boost::optional<Camera*>());

		}*/

		void BindScene(State state)
		{

			state
			[

				Class<GameObjectRayIntersection>("GameObjectRayIntersection").

					Property("hObj", &GameObjectRayIntersection::hObj).
					Property("t", &GameObjectRayIntersection::t),

				Class<SceneModule>("SceneModule").

					Function("AddGameObject", &SceneModule::AddGameObject).
					Function("RemoveGameObject", &SceneModule::RemoveGameObject).
					Function("Pick", &SceneModule::Pick).

					Property("camera", (Camera * (SceneModule::*) ()) &SceneModule::GetCamera, &SceneModule::SetCamera)

			];

			/*module(L)
			[

				class_<GameObjectRayIntersection>("GameObjectRayIntersection").
					def_readwrite("hObj", &GameObjectRayIntersection::hObj).
					def_readwrite("t",    &GameObjectRayIntersection::t),

				class_<SceneModule>("SceneModule").

				def("AddGameObject", &SceneModule::AddGameObject).
					def("RemoveGameObject", &SceneModule::RemoveGameObject).

					def("Pick", &SceneModule::Pick).

					property("camera", (Camera* (SceneModule::*) ()) &SceneModule::GetCamera, &SceneModule::SetCamera)

			];*/

		}

	}

}