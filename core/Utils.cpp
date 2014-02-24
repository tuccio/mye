#include "Utils.h"

#include <sstream>

using namespace std;

namespace mye
{

	namespace core
	{

		String PointerToString(void *p)
		{
			stringstream ss;
			ss << p;
			return ss.str().c_str();
		}

		void* StringToPointer(const String &str)
		{
			std::stringstream ss(str.CString());
			void *ptr;
			ss >> ptr;
			return ptr;
		}

	}

}