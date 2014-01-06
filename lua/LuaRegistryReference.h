#pragma once

#include <lua.hpp>

namespace mye
{

	namespace lua
	{

		struct LuaRegistryReference
		{

			LuaRegistryReference(lua_State *L);
			LuaRegistryReference(lua_State *L,
				int index);

			~LuaRegistryReference(void);

			operator int () const;

			lua_State *L;
			int ref;

		};

	}

}

