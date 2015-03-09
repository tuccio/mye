#include "Physics.h"

#include <mye/core/PhysicsModule.h>

using namespace mye::lua;
using namespace mye::core;
using namespace luapp11;

namespace mye
{

	namespace lua
	{

		void BindPhysics(luapp11::State state)
		{

			BindBullet(state);

			state
			[

				Class<PhysicsModule>("__PhysicsModule").
					Function("GetBodiesCount", &PhysicsModule::GetBodiesCount).
					Property("gravity", &PhysicsModule::GetGravity, &PhysicsModule::SetGravity)

			];

		}

	}

}