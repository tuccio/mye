#include "InputModule.h"
#include "Types.h"

#include <mye/core/Game.h>
#include <mye/core/Keyboard.h>
#include <mye/core/Mouse.h>

#include <boost/ref.hpp>

#include <luabind/luabind.hpp>

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		boost::reference_wrapper<Keyboard> __in_get_keyboard(InputModule &input);

		boost::reference_wrapper<Mouse> __in_get_mouse(InputModule &input);

		void BindInputModule(lua_State *L)
		{

			module(L)
			[

				class_<InputModule>(MYE_LUA_INPUTMODULE).

				def("HasKeyboard", &InputModule::HasKeyboard).
				def("Keyboard", &__in_get_keyboard)

			];

		}

		void BindKeyboard(lua_State *L)
		{

			module(L)
			[

				class_<Keyboard>(MYE_LUA_KEYBOARD).

					enum_("KeyboardVK")
					[
						value("A", KeyboardVK::MYE_VK_A),
						value("B", KeyboardVK::MYE_VK_B),
						value("C", KeyboardVK::MYE_VK_C)
					].

					def("IsPressed", &Keyboard::IsPressed)

			];

		}

		boost::reference_wrapper<Keyboard> __in_get_keyboard(InputModule &input)
		{
			return boost::ref(*input.GetKeyboard());
		}

		boost::reference_wrapper<Mouse> __in_get_mouse(InputModule &input)
		{
			return boost::ref(*input.GetMouse());
		}

	}

}