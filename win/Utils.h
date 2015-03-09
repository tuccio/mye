#include <mye/core/String.h>
#include <Windows.h>

namespace mye
{

	namespace win
	{

		mye::core::String GetLastErrorAsString(void);

		mye::core::String FormatSystemTime(const SYSTEMTIME *lpTime,
			const mye::core::String &format);

		void ShowErrorBox(const mye::core::String &message = GetLastErrorAsString(),
			HWND hParent = 0x0);

		bool MakeTransparentBitmap(HBITMAP hBitmap, HDC hDC);

		inline bool CreateConsole(void)
		{

			if (AllocConsole())
			{
				freopen("CONOUT$", "w", stdout);
				freopen("CONIN$", "r", stdin);
				return true;
			}

			return false;

		}

		inline void DestroyConsole(void)
		{

			FreeConsole();

		}

		inline void FocusConsole(void)
		{

			HWND hWnd = GetConsoleWindow();

			if (hWnd)
			{

				SetFocus(hWnd);
				ShowWindow(hWnd, TRUE);
				SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

			}

		}

		inline void PopupMessage(const char *title, const char *msg)
		{
			::MessageBox(NULL, msg, title, MB_OK);
		}

	}

}