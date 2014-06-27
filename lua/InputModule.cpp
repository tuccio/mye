#include "InputModule.h"
#include "Types.h"
#include "Converters.h"
#include "Alignment.h"

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
					property("keyboard", &__in_get_keyboard).
					property("mouse", &__in_get_mouse),

				class_<Keyboard>(MYE_LUA_KEYBOARD).

					enum_("KeyboardVK")
					[

						value("A", KeyboardVK::MYE_VK_A),
						value("B", KeyboardVK::MYE_VK_B),
						value("C", KeyboardVK::MYE_VK_C),
						value("D", KeyboardVK::MYE_VK_D),
						value("E", KeyboardVK::MYE_VK_E),
						value("F", KeyboardVK::MYE_VK_F),
						value("G", KeyboardVK::MYE_VK_G),
						value("H", KeyboardVK::MYE_VK_H),
						value("I", KeyboardVK::MYE_VK_I),
						value("J", KeyboardVK::MYE_VK_J),
						value("K", KeyboardVK::MYE_VK_K),
						value("L", KeyboardVK::MYE_VK_L),
						value("M", KeyboardVK::MYE_VK_M),
						value("N", KeyboardVK::MYE_VK_N),
						value("O", KeyboardVK::MYE_VK_O),
						value("P", KeyboardVK::MYE_VK_P),
						value("Q", KeyboardVK::MYE_VK_Q),
						value("R", KeyboardVK::MYE_VK_R),
						value("S", KeyboardVK::MYE_VK_S),
						value("T", KeyboardVK::MYE_VK_T),
						value("U", KeyboardVK::MYE_VK_U),
						value("V", KeyboardVK::MYE_VK_V),
						value("W", KeyboardVK::MYE_VK_W),
						value("X", KeyboardVK::MYE_VK_X),
						value("Y", KeyboardVK::MYE_VK_Y),
						value("Z", KeyboardVK::MYE_VK_Z),
						value("Space", KeyboardVK::MYE_VK_SPACE),
						value("Enter", KeyboardVK::MYE_VK_ENTER),
						value("Esc", KeyboardVK::MYE_VK_ESC),
						value("Shift", KeyboardVK::MYE_VK_LSHIFT),
						value("RightShift", KeyboardVK::MYE_VK_RSHIFT),
						value("Alt", KeyboardVK::MYE_VK_LALT),
						value("RightAlt", KeyboardVK::MYE_VK_RALT),
						value("Ctrl", KeyboardVK::MYE_VK_LCTRL),
						value("RightCtrl", KeyboardVK::MYE_VK_RCTRL),
						value("Tab", KeyboardVK::MYE_VK_TAB),
						value("Backspace", KeyboardVK::MYE_VK_BACKSPACE),
						value("LeftArrow", KeyboardVK::MYE_VK_LEFT_ARROW),
						value("RightArrow", KeyboardVK::MYE_VK_RIGHT_ARROW),
						value("UpArrow", KeyboardVK::MYE_VK_UP_ARROW),
						value("DownArrow", KeyboardVK::MYE_VK_DOWN_ARROW)

					].

					def("IsPressed", &Keyboard::IsPressed),

					class_<Mouse>(MYE_LUA_MOUSE).

					enum_("MouseVK")
					[
						value("Left", MouseVK::MYE_VK_MOUSE_LEFT),
						value("Right", MouseVK::MYE_VK_MOUSE_RIGHT),
						value("Middle", MouseVK::MYE_VK_MOUSE_MIDDLE),
						value("Mouse1", MouseVK::MYE_VK_MOUSE_LEFT),
						value("Mouse2", MouseVK::MYE_VK_MOUSE_RIGHT),
						value("Mouse3", MouseVK::MYE_VK_MOUSE_MIDDLE)
					].

					property("position", &Mouse::GetPosition).
					property("delta", &Mouse::GetDelta).
					property("wheel", &Mouse::GetWheelDelta).

					def("IsPressed", &Mouse::IsPressed)

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