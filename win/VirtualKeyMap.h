#pragma once

#include <mye/core/VirtualKeys.h>
#include <Windows.h>

#define __MYE_MAKE_KEYBOARDVK(__BASE, __SHIFT) mye::core::KeyboardVK(mye::core::KeyboardVK::__BASE + (__SHIFT))

namespace mye
{
	namespace win
	{

		inline mye::core::KeyboardVK VirtualKeyMap(WPARAM wParam, LPARAM lParam)
		{

			if (wParam >= 0x41 && wParam <= 0x5A)
			{
				return __MYE_MAKE_KEYBOARDVK(MYE_VK_A, wParam - 0x41);
			}

			if (wParam >= 0x30 && wParam <= 0x39)
			{
				return __MYE_MAKE_KEYBOARDVK(MYE_VK_0, wParam - 0x30);
			}

			return mye::core::KeyboardVK::MYE_VK_COUNT;

		}

	}
}