#include "Utils.h"

#include <Windows.h>
#include <vector>

using namespace mye::core;

namespace mye
{

	namespace win
	{

		String GetLastErrorAsString(void)
		{

			LPTSTR error = nullptr;
			String rError;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				nullptr,
				GetLastError(),
				LANG_NEUTRAL,
				(LPTSTR) &error,
				0,
				nullptr);

			if (error)
			{
				rError = error;
				LocalFree(error);
			}

			String::Position pos = rError.FindLast("\r\n");

			if (pos != String::Null)
			{
				rError.Resize(pos);
			}

			return rError;

		}

		mye::core::String FormatSystemTime(const SYSTEMTIME *lpTime,
			const mye::core::String &format)
		{

			int len = GetTimeFormat(LOCALE_INVARIANT,
				TIME_FORCE24HOURFORMAT,
				lpTime,
				format.CString(),
				nullptr,
				0);

			mye::core::String r(len);

			GetTimeFormat(LOCALE_INVARIANT,
				TIME_FORCE24HOURFORMAT,
				lpTime,
				format.CString(),
				&r[0],
				0);

			r.UpdateLength();

			return r;

		}

		void ShowErrorBox(const mye::core::String &message)
		{
			MessageBox(nullptr,
				message.CString(),
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