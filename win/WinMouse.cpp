#include "VirtualKeyMap.h"
#include "WinMouse.h"

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

			{

				HWND hActiveWindow = GetActiveWindow();

				if (hActiveWindow)
				{

					RECT windowRect, clientRect;

					GetWindowRect(hActiveWindow, &windowRect);
					GetClientRect(hActiveWindow, &clientRect);

					if (clientRect.bottom != 0 && clientRect.right != 0)
					{

						LONG windowWidth = windowRect.right - windowRect.left;
						LONG windowHeight = windowRect.bottom - windowRect.top;

						Vector2 padding((windowWidth - clientRect.right) * 0.5f, (windowHeight - clientRect.bottom) * 0.5f);

						Vector2 position((float) (mhookStruct->pt.x - windowRect.left) / (padding.x() + clientRect.right),
						                 (float) 1.0f - (windowRect.bottom - mhookStruct->pt.y) / (padding.y() + clientRect.bottom));

						//WinMouse::GetSingleton().Move(position);
						WinMouse::GetSingleton().MoveNextFrame(position);

					}

				}

			}
			
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

	return CallNextHookEx(nullptr, nCode, wParam, lParam);

}

bool WinMouse::Hook(void)
{

	m_hHook = SetWindowsHookEx(
		WH_MOUSE,
		&__mye_winmouse_hook,
		nullptr,
		GetCurrentThreadId());

	return (m_hHook == nullptr ? false : true);

}

void WinMouse::Unhook(void)
{
	UnhookWindowsHookEx(m_hHook);
}