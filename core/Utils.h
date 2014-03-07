#pragma once

#include "String.h"
#include <vector>

namespace mye
{

	namespace core
	{

		String PointerToString(const void *p);
		const void* StringToPointer(const String &str);

		void RuntimeError(const String &error);

		std::vector<String> SplitCommandLine(const String &cmd);

	}
}