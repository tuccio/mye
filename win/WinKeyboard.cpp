#include "WinKeyboard.h"
#include "VirtualKeyMap.h"

//#define MYE_KEYBOARD_LL

using namespace mye::core;
using namespace mye::win;

WinKeyboard::WinKeyboard(void)
{
}


WinKeyboard::~WinKeyboard(void)
{
	Unhook();
}

#ifdef MYE_KEYBOARD_LL

LRESULT __stdcall __mye_winkeyboard_llhook(int nCode,
											WPARAM wParam,
											LPARAM lParam)
{

	if (nCode >= 0)
	{

		PKBDLLHOOKSTRUCT kbHookStruct = (PKBDLLHOOKSTRUCT) lParam;

		KeyboardVK key = VirtualKeyMap(kbHookStruct->vkCode);

		if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
		{
			WinKeyboard::GetSingleton().Release(key);
		}
		else if (wParam == WM_SYSKEYUP || wParam == WM_KEYUP)
		{
			WinKeyboard::GetSingleton().Press(key);
		}

	}

	return CallNextHookEx(nullptr, nCode, wParam, lParam);

}

#else

LRESULT __stdcall __mye_winkeyboard_hook(int nCode,
										 WPARAM wParam,
										 LPARAM lParam)
{

	if (nCode >= 0)
	{

		KeyboardVK key = VirtualKeyMap(wParam);

		if (HIWORD(lParam) & KF_UP)
		{
			WinKeyboard::GetSingleton().Release(key);
		}
		else
		{
			WinKeyboard::GetSingleton().Press(key);
		}

	}

	return CallNextHookEx(nullptr, nCode, wParam, lParam);

}

#endif

bool WinKeyboard::Hook(void)
{

#ifdef MYE_KEYBOARD_LL

	m_hHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		&__mye_winkeyboard_llhook,
		nullptr,
		0);

#else

	m_hHook = SetWindowsHookEx(
		WH_KEYBOARD,
		&__mye_winkeyboard_hook,
		nullptr,
		GetCurrentThreadId());

#endif

	return (m_hHook == nullptr ? false : true);

}

void WinKeyboard::Unhook(void)
{
	UnhookWindowsHookEx(m_hHook);
}