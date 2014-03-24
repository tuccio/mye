#include "Physics.h"

#include <mye/core/PhysicsModule.h>

using namespace mye::lua;
using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		void BindPhysics(lua_State *L)
		{

			BindBullet(L);

			module(L)
			[

				class_<PhysicsModule>("__PhysicsModule").
					property("gravity", &PhysicsModule::GetGravity, &PhysicsModule::SetGravity)

			];

		}

	}

}