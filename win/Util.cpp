#include "Util.h"
#include <Windows.h>

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
				LANG_SYSTEM_DEFAULT,
				(LPTSTR) &error,
				0,
				NULL);

			if (error)
			{
				rError = error;
				LocalFree(error);
			}

			return rError;

		}

	}

}