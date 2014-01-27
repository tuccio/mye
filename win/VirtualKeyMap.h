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

			case VK_LMENU:
			case VK_MENU:
				return mye::core::KeyboardVK::MYE_VK_LALT;
				break;

			default:
				return mye::core::KeyboardVK::MYE_VK_COUNT;

			}

		}

	}
}