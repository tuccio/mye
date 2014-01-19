#include "Utils.h"

#include <sstream>

using namespace std;

namespace mye
{

	namespace core
	{

		string PointerToString(void *p)
		{
			stringstream ss;
			ss << p;
			return ss.str();
		}

		void* StringToPointer(const std::string &str)
		{
			std::stringstream ss(str);
			void *ptr;
			ss >> ptr;
			return ptr;
		}

	}

}