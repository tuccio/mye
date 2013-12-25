#pragma once

#include <lua.hpp>

namespace mye
{

	namespace lua
	{

		void RegisterWindow(lua_State *L);
		void RegisterApplication(lua_State *L);

	}

}