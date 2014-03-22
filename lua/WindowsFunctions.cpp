#pragma once

#include "Types.h"
#include "WindowsFunctions.h"
#include "Converters.h"

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

		void __win_alloc_console(void);
		void __win_free_console(void);
		void __win_message_box(const char *title, const char *msg);

		void BindWindowsFunctions(lua_State *L)
		{

			module(L, MYE_LUA_WIN)
			[
				
				class_<mye::win::Window, IWindow>(MYE_LUA_WINDOW).
					def(constructor<>()),

				def("AllocConsole", &__win_alloc_console),
				def("FreeConsole", &__win_free_console),

				def("MessageBox", &__win_message_box)

			];

		}

		void __win_alloc_console(void)
		{

			if (AllocConsole())
			{
				freopen("CONOUT$", "w", stdout);
			}
			
		}

		void __win_free_console(void)
		{
			FreeConsole();
		}

		void __win_message_box(const char *title, const char *msg)
		{
			MessageBox(NULL, msg, title, MB_OK);
		}

	}

}