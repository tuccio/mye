#include "WinMouse.h"
#include "VirtualKeyMap.h"

using namespace mye::math;
using namespace mye::core;
using namespace mye::win;

WinMouse::WinMouse(void)
{
}


WinMouse::~WinMouse(void)
{
	Unhook();
}

LRESULT __stdcall __mye_winmouse_hook(int nCode,
									  WPARAM wParam,
									  LPARAM lParam)
{

	if (nCode >= 0)
	{

		PMOUSEHOOKSTRUCT mhookStruct = (PMOUSEHOOKSTRUCT) (lParam);
		switch (wParam)
		{

		case WM_MOUSEWHEEL:
			
			{
				short wheel = HIWORD(((PMOUSEHOOKSTRUCTEX) mhookStruct)->mouseData);
				WinMouse::GetSingleton().SetWheelDelta(wheel / WHEEL_DELTA);
			}
			
			break;

		case WM_MOUSEMOVE:
			WinMouse::GetSingleton().
				Move(Vector2f(mhookStruct->pt.x, mhookStruct->pt.y));
			break;

		case WM_LBUTTONDOWN:
			WinMouse::GetSingleton().Press(MYE_VK_MOUSE_LEFT);
			break;

		case WM_LBUTTONUP:
			WinMouse::GetSingleton().Release(MYE_VK_MOUSE_LEFT);
			break;

		case WM_RBUTTONDOWN:
			WinMouse::GetSingleton().Press(MYE_VK_MOUSE_RIGHT);
			break;

		case WM_RBUTTONUP:
			WinMouse::GetSingleton().Release(MYE_VK_MOUSE_RIGHT);
			break;

		case WM_MBUTTONDOWN:
			WinMouse::GetSingleton().Press(MYE_VK_MOUSE_MIDDLE);
			break;

		case WM_MBUTTONUP:
			WinMouse::GetSingleton().Release(MYE_VK_MOUSE_MIDDLE);
			break;

		default:
			break;

		}

	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);

}

bool WinMouse::Hook(void)
{

	m_hHook = SetWindowsHookEx(
		WH_MOUSE,
		&__mye_winmouse_hook,
		NULL,
		GetCurrentThreadId());

	return (m_hHook == NULL ? false : true);

}

void WinMouse::Unhook(void)
{
	UnhookWindowsHookEx(m_hHook);
}