#pragma once

#include <lua.hpp>
#include <luabind/luabind.hpp>

#define LPLUASTATE (((mye::lua::LuaModule*) mye::core::Game::GetSingleton().GetScriptModule())->GetLuaState())

namespace mye
{

	namespace lua
	{

		class LuaStackCleaner
		{

		public:

			LuaStackCleaner(lua_State *lua);
			~LuaStackCleaner(void);

		private:

			lua_State * m_lua;
			int m_top;

		};

	}

}