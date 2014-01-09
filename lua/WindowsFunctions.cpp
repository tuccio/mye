#pragma once

#include "Types.h"
#include "WindowsFunctions.h"

#include <Windows.h>

#include <luabind/luabind.hpp>

#include <mye/core/IWindow.h>
#include <mye/win/Window.h>

using namespace luabind;
using namespace mye::core;
using namespace mye::win;

namespace mye
{

	namespace lua
	{

		int __win_alloc_console(lua_State *L);
		int __win_free_console(lua_State *L);

		void BindWindowsFunctions(lua_State *L)
		{

			module(L, MYE_LUA_WIN)
			[
				
				class_<mye::win::Window, IWindow>(MYE_LUA_WINDOW).
					def(constructor<>()),

				def("AllocConsole", &__win_alloc_console),
				def("FreeConsole", &__win_free_console)

			];

		}

		int __win_alloc_console(lua_State *L)
		{

			if (AllocConsole())
			{
				freopen("CONOUT$", "w", stdout);
			}
			
			return 0;
		}

		int __win_free_console(lua_State *L)
		{
			FreeConsole();
			return 0;
		}

	}

}