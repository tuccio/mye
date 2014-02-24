#pragma once

#include "String.h"

namespace mye
{

	namespace core
	{

		String PointerToString(void *p);
		void* StringToPointer(const String &str);

	}
}