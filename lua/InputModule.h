#pragma once

#include <lua.hpp>

namespace mye
{
	namespace lua
	{

		void BindInputModule(lua_State *L);
		void BindKeyboard(lua_State *L);
		void BindMosue(lua_State *L);

	}
}