#include "Utils.h"

#include <Windows.h>
#include <vector>

namespace mye
{

	namespace win
	{

		std::string GetLastErrorAsString(void)
		{

			LPTSTR error = NULL;
			std::string rError;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				LANG_NEUTRAL,
				(LPTSTR) &error,
				0,
				NULL);

			if (error)
			{
				rError = error;
				LocalFree(error);
			}

			size_t pos = rError.find_last_not_of("\r\n");

			if (pos != std::string::npos)
			{
				rError.resize(pos);
			}

			return rError;

		}

		std::string FormatSystemTime(const SYSTEMTIME *lpTime,
			const std::string &format)
		{

			int len = GetTimeFormat(LOCALE_INVARIANT,
				TIME_FORCE24HOURFORMAT,
				lpTime,
				format.c_str(),
				NULL,
				0);

			char *str = new char[len + 1];

			GetTimeFormat(LOCALE_INVARIANT,
				TIME_FORCE24HOURFORMAT,
				lpTime,
				format.c_str(),
				str,
				0);

			std::string result(str);

			delete str;

			return result;

		}

		void ShowErrorBox(const std::string &message)
		{
			MessageBox(NULL,
				message.c_str(),
				"Error",
				MB_OK | MB_ICONERROR);
		}

		bool MakeTransparentBitmap(HBITMAP hBitmap, HDC hDC)
		{

			bool success = false;

			BITMAP bitmap;

			if (GetObject(hBitmap, sizeof(BITMAP), &bitmap) &&
				bitmap.bmWidthBytes > 0 &&
				bitmap.bmHeight > 0)
			{

				BITMAPINFO bitmapInfo;
				size_t cb = bitmap.bmWidthBytes * bitmap.bmHeight;

				std::vector<unsigned char> bitmapBits(cb);
				size_t s = GetBitmapBits(hBitmap, cb, &bitmapBits[0]);

				if (GetDIBits(hDC,
					hBitmap,
					0,
					bitmap.bmHeight,
					&bitmapBits[0],
					&bitmapInfo,
					DIB_RGB_COLORS))
				{

					DWORD dwColorWindow = GetSysColor(COLOR_WINDOW);
					DWORD dwColorTransparent = RGB(bitmapBits[0], bitmapBits[1], bitmapBits[2]);

					if (dwColorWindow != dwColorTransparent)
					{

						unsigned char colorWindowR = GetRValue(dwColorWindow);
						unsigned char colorWindowG = GetGValue(dwColorWindow);
						unsigned char colorWindowB = GetBValue(dwColorWindow);

						unsigned char colorTransparentR = GetRValue(dwColorTransparent);
						unsigned char colorTransparentG = GetGValue(dwColorTransparent);
						unsigned char colorTransparentB = GetBValue(dwColorTransparent);

						int colors = bitmap.bmHeight * bitmap.bmWidth;

						for (int i = 0; i < colors; i += 3)
						{

							unsigned char &currentColorR = bitmapBits[i];
							unsigned char &currentColorG = bitmapBits[i + 1];
							unsigned char &currentColorB = bitmapBits[i + 2];

							if (colorTransparentR == currentColorR &&
								colorTransparentG == currentColorG &&
								colorTransparentB == currentColorB)
							{
								currentColorR = colorWindowR;
								currentColorG = colorWindowG;
								currentColorB = colorWindowB;
							}

						}

						//SetBitmapBits(hBitmap, cb, &bitmapBits[0]);
						if (SetDIBits(hDC,
							hBitmap,
							0,
							bitmap.bmHeight,
							&bitmapBits[0],
							&bitmapInfo,
							DIB_RGB_COLORS))
						{
							success = true;
						}

					}

				}

			}

			return success;
			
		}

		

	}

}