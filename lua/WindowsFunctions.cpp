#pragma once

#include "Types.h"
#include "WindowsFunctions.h"
#include "Converters.h"

#include <Windows.h>

#include <mye/core/IWindow.h>
#include <mye/win/Window.h>
#include <mye/win/Utils.h>

using namespace luapp11;
using namespace mye::core;

namespace mye
{

	namespace lua
	{

		void BindWindowsFunctions(State state)
		{

			state
			[

				Namespace(MYE_LUA_WIN)
				[

					Class<mye::win::Window, IWindow>(MYE_LUA_WINDOW).
						Constructor<>(),
					
					Function("CreateConsole",  &mye::win::CreateConsole),
					Function("DestroyConsole", &mye::win::DestroyConsole),
					Function("FocusConsole",   &mye::win::FocusConsole),

					Function("PopupMessage",   &mye::win::PopupMessage)

				]

			];

			auto globals = state.GetGlobalTable();

			globals["System"] = globals[MYE_LUA_WIN];

			/*module(L, MYE_LUA_WIN)
			[
				
				class_<mye::win::Window, IWindow>(MYE_LUA_WINDOW).
					def(constructor<>()),

				def("CreateConsole", &mye::win::CreateConsole),
				def("DestroyConsole", &mye::win::DestroyConsole),
				def("FocusConsole", &mye::win::FocusConsole),

				def("MessageBox", &mye::win::MessageBox)

			];*/

		}

		

	}

}