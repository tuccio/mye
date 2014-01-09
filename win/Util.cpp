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

	}

}