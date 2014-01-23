#pragma once

#include <mye/core/Game.h>

namespace mye
{

	namespace lua
	{

		void BindGame(lua_State *L);
		void BindWindow(lua_State *L);

	}

}