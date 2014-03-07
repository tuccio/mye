#include "Utils.h"
#include "Game.h"

#include <sstream>
#include <regex>

using namespace std;

namespace mye
{

	namespace core
	{

		String PointerToString(const void *p)
		{
			stringstream ss;
			ss << p;
			return ss.str().c_str();
		}

		const void* StringToPointer(const String &str)
		{
			std::stringstream ss(str.CString());
			void *ptr;
			ss >> ptr;
			return ptr;
		}

		void RuntimeError(const String &error)
		{
			Game::GetSingleton().RuntimeError(error);
		}

		std::vector<String> SplitCommandLine(const String &cmd)
		{

			std::vector<String> result;

			std::smatch sm;
			
			std::regex r("(\"([^\"]*)\")|([^\\s\")]+)(\\s+|$)");
			std::string line(cmd.CString());


			while(std::regex_search(line, sm, r))
			{

				size_t i = (sm[2].length() ? 2 : 3);

				result.push_back(sm[i].str().c_str());

				line = sm.suffix().str();

			}

			return result;

		}

	}

}