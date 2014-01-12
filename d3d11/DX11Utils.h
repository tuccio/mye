#pragma once

#include <Windows.h>

#if defined(DEBUG) | defined(_DEBUG)

#ifndef HRDEBUG
#define HRDEBUG(hr)\
		if(FAILED(hr))\
		{\
			LPTSTR output;\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,\
			NULL,\
			hr,\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
			(LPTSTR) &output,\
			0x0,\
			NULL);\
			MessageBox(NULL, output, "DirectX Error", MB_OK);\
		}
#endif
#else
#ifndef HRDEBUG
#define HRDEBUG(x)
#endif
#endif

#ifndef ReleaseCOM(x)
#define ReleaseCOM(x) x->Release(); x = 0x0;
#endif