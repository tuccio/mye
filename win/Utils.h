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

	}

}