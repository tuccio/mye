#include "InputModule.h"
#include "Types.h"
#include "Converters.h"

#include <mye/core/Game.h>
#include <mye/core/Keyboard.h>
#include <mye/core/Mouse.h>

using namespace mye::core;
using namespace luapp11;

namespace mye
{

	namespace lua
	{

		void BindInputModule(luapp11::State state)
		{

			state
			[

				Class<InputModule>(MYE_LUA_INPUTMODULE).

					Function("HasKeyboard", &InputModule::HasKeyboard).
					Property("keyboard",    (Keyboard * (InputModule::*) (void)) &InputModule::GetKeyboard).
					Property("mouse",       (Mouse * (InputModule::*) (void)) &InputModule::GetMouse),

				Enum<KeyboardVK>("KeyboardVK")
					("A", KeyboardVK::MYE_VK_A)
					("B", KeyboardVK::MYE_VK_B)
					("C", KeyboardVK::MYE_VK_C)
					("D", KeyboardVK::MYE_VK_D)
					("E", KeyboardVK::MYE_VK_E)
					("F", KeyboardVK::MYE_VK_F)
					("G", KeyboardVK::MYE_VK_G)
					("H", KeyboardVK::MYE_VK_H)
					("I", KeyboardVK::MYE_VK_I)
					("J", KeyboardVK::MYE_VK_J)
					("K", KeyboardVK::MYE_VK_K)
					("L", KeyboardVK::MYE_VK_L)
					("M", KeyboardVK::MYE_VK_M)
					("N", KeyboardVK::MYE_VK_N)
					("O", KeyboardVK::MYE_VK_O)
					("P", KeyboardVK::MYE_VK_P)
					("Q", KeyboardVK::MYE_VK_Q)
					("R", KeyboardVK::MYE_VK_R)
					("S", KeyboardVK::MYE_VK_S)
					("T", KeyboardVK::MYE_VK_T)
					("U", KeyboardVK::MYE_VK_U)
					("V", KeyboardVK::MYE_VK_V)
					("W", KeyboardVK::MYE_VK_W)
					("X", KeyboardVK::MYE_VK_X)
					("Y", KeyboardVK::MYE_VK_Y)
					("Z", KeyboardVK::MYE_VK_Z)

					("Space",      KeyboardVK::MYE_VK_SPACE)
					("Enter",      KeyboardVK::MYE_VK_ENTER)
					("Esc",        KeyboardVK::MYE_VK_ESC)
					("Shift",      KeyboardVK::MYE_VK_LSHIFT)
					("RightShift", KeyboardVK::MYE_VK_RSHIFT)
					("Alt",        KeyboardVK::MYE_VK_LALT)
					("RightAlt",   KeyboardVK::MYE_VK_RALT)
					("Ctrl",       KeyboardVK::MYE_VK_LCTRL)
					("RightCtrl",  KeyboardVK::MYE_VK_RCTRL)
					("Tab",        KeyboardVK::MYE_VK_TAB)
					("Backspace",  KeyboardVK::MYE_VK_BACKSPACE)
					("LeftArrow",  KeyboardVK::MYE_VK_LEFT_ARROW)
					("RightArrow", KeyboardVK::MYE_VK_RIGHT_ARROW)
					("UpArrow",    KeyboardVK::MYE_VK_UP_ARROW)
					("DownArrow",  KeyboardVK::MYE_VK_DOWN_ARROW)

					("F1",  KeyboardVK::MYE_VK_F1)
					("F2",  KeyboardVK::MYE_VK_F2)
					("F3",  KeyboardVK::MYE_VK_F3)
					("F4",  KeyboardVK::MYE_VK_F4)
					("F5",  KeyboardVK::MYE_VK_F5)
					("F6",  KeyboardVK::MYE_VK_F6)
					("F7",  KeyboardVK::MYE_VK_F7)
					("F8",  KeyboardVK::MYE_VK_F8)
					("F9",  KeyboardVK::MYE_VK_F9)
					("F10", KeyboardVK::MYE_VK_F10)
					("F11", KeyboardVK::MYE_VK_F11)
					("F12", KeyboardVK::MYE_VK_F12),

				Class<Keyboard>(MYE_LUA_KEYBOARD).

					Function("IsPressed", &Keyboard::IsPressed),

				Enum<MouseVK>("MouseVK")

					("Left",   MouseVK::MYE_VK_MOUSE_LEFT)
					("Right",  MouseVK::MYE_VK_MOUSE_RIGHT)
					("Middle", MouseVK::MYE_VK_MOUSE_MIDDLE)
					("Mouse1", MouseVK::MYE_VK_MOUSE_LEFT)
					("Mouse2", MouseVK::MYE_VK_MOUSE_RIGHT)
					("Mouse3", MouseVK::MYE_VK_MOUSE_MIDDLE),

				Class<Mouse>(MYE_LUA_MOUSE).

					Property("position",  &Mouse::GetPosition).
					Property("delta",     &Mouse::GetDelta).
					Property("wheel",     &Mouse::GetWheelDelta).

					Function("IsPressed", &Mouse::IsPressed)

			];

		}

	}

}