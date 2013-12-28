#pragma once

#include <mye/core/Game.h>
#include <lua.hpp>

#include <string>

namespace mye
{

	namespace lua
	{

		class LuaEnvironment
		{

		public:

			LuaEnvironment(void);
			~LuaEnvironment(void);

			lua_State* GetLuaState(void);

			bool RunFile(const std::string &file);
			bool RunString(const std::string &code);

			std::string GetLastError(void) const;

			void SetGameInstance(mye::core::Game &game);

		private:

			void OpenAllLibraries(void);

			lua_State *_L;
			std::string _lastError;
			mye::core::Game *_game;

		};

	}

}

