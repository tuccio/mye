#pragma once

#include <string>

namespace mye
{

	namespace core
	{

		std::string PointerToString(void *p);
		void* StringToPointer(const std::string &str);

	}
}