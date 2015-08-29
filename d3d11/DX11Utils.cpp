#include "DX11Utils.h"
#include <cassert>
#include <mye/win/Utils.h>

namespace mye
{
	namespace dx11
	{
		bool HRTestFailed(HRESULT hr)
		{

			if (FAILED(hr))
			{

				LPTSTR output;

				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,
					hr,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &output,
					0x0,
					nullptr);

				mye::win::ShowErrorBox(output);

				return true;

			}

			return false;

		}
	}
}