#include "Windows.h"
#include "Types.h"

#include <mye/win/Window.h>
#include <mye/win/Application.h>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

using namespace luabind;
using namespace Eigen;
using namespace mye::win;

namespace mye
{

	namespace lua
	{

		void RegisterWindow(lua_State *L)
		{

			module(L)
			[
				
				class_<Window>(MYE_LUA_WINDOW).

				def(constructor<>()).

				def("Create", (bool (Window::*) (void)) &Window::Create).
				def("Destroy", &Window::Destroy).

				def("Exists", &Window::Exists).
				def("IsVisible", &Window::IsVisible).

				def("IsMinimized", &Window::IsMinimized).

				def("Show", &Window::Show).
				def("Hide", &Window::Hide).

				def("SetCaption", &Window::SetCaption).
				def("GetCaption", &Window::GetCaption).

				def("SetSize", &Window::SetSize).
				def("GetSize", &Window::GetSize).

				def("SetPosition", &Window::SetPosition).
				def("GetPosition", &Window::GetPosition)
				

			];

		}

		void RegisterApplication(lua_State *L)
		{

			module(L)
			[

				class_<Application>(MYE_LUA_APPLICATION).

				def(constructor<>()).

				def("GetMainWindow", (Window& (Application::*) (void)) &Application::GetMainWindow).
				def("Run", &Application::Run)

			];

		}

	}

}