#include "Physics.h"

using namespace mye::lua;

namespace mye
{

	namespace lua
	{

		void BindPhysics(lua_State *L)
		{

			BindBullet(L);

		}

	}

}