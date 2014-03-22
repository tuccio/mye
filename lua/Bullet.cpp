#include "Bullet.h"
#include "Types.h"
#include "Converters.h"

using namespace mye::lua;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		void BindBullet(lua_State *L)
		{

			module(L, "Physics")
			[

				/* Shapes */

				class_<btCollisionShape>(MYE_LUA_BULLET_SHAPE),

				class_<btSphereShape, btCollisionShape>(MYE_LUA_BULLET_SPHERE),

				class_<btBoxShape, btCollisionShape>(MYE_LUA_BULLET_BOX).
					def(constructor<const btVector3&>())

			];

		}

	}

}