#pragma once

#include <lua.hpp>
#include <string>

namespace mye
{

	namespace lua
	{

		class Interpreter
		{

		public:

			Interpreter(void);
			~Interpreter(void);

			bool Run(const std::string &file);

			std::string GetLastError(void);

		private:

			lua_State *_L;
			std::string _lastError;

		};

	}

}
