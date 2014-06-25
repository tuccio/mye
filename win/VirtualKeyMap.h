#pragma once

#include <mye/core/VirtualKeys.h>
#include <Windows.h>

#define __MYE_MAKE_KEYBOARDVK(__BASE, __SHIFT) mye::core::KeyboardVK(mye::core::KeyboardVK::__BASE + (__SHIFT))

namespace mye
{
	namespace win
	{

		inline mye::core::KeyboardVK VirtualKeyMap(WPARAM vkCode)
		{

			if (vkCode >= 0x41 && vkCode <= 0x5A)
			{
				return __MYE_MAKE_KEYBOARDVK(MYE_VK_A, vkCode - 0x41);
			}

			if (vkCode >= 0x30 && vkCode <= 0x39)
			{
				return __MYE_MAKE_KEYBOARDVK(MYE_VK_0, vkCode - 0x30);
			}

			switch (vkCode)
			{

			case VK_LSHIFT:
			case VK_SHIFT:
				return mye::core::KeyboardVK::MYE_VK_LSHIFT;

			case VK_RSHIFT:
				return mye::core::KeyboardVK::MYE_VK_RSHIFT;

			case VK_LMENU:
			case VK_MENU:
				return mye::core::KeyboardVK::MYE_VK_LALT;

			case VK_RMENU:

				return mye::core::KeyboardVK::MYE_VK_RALT;

			case VK_LCONTROL:
			case VK_CONTROL:
				return mye::core::KeyboardVK::MYE_VK_LCTRL;

			case VK_RCONTROL:
				return mye::core::KeyboardVK::MYE_VK_RCTRL;

			case VK_SPACE:
				return mye::core::KeyboardVK::MYE_VK_SPACE;

			case VK_LEFT:
				return mye::core::KeyboardVK::MYE_VK_LEFT_ARROW;

			case VK_RIGHT:
				return mye::core::KeyboardVK::MYE_VK_RIGHT_ARROW;

			case VK_UP:
				return mye::core::KeyboardVK::MYE_VK_UP_ARROW;

			case VK_DOWN:
				return mye::core::KeyboardVK::MYE_VK_DOWN_ARROW;
				

			default:
				return mye::core::KeyboardVK::MYE_VK_COUNT;

			}

		}

	}
}