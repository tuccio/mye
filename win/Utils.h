#include <string>

#include <Windows.h>

namespace mye
{

	namespace win
	{

		std::string GetLastErrorAsString(void);

		std::string FormatSystemTime(const SYSTEMTIME *lpTime,
			const std::string &format);

		void ShowErrorBox(const std::string &message = GetLastErrorAsString());

		bool MakeTransparentBitmap(HBITMAP hBitmap, HDC hDC);

	}

}