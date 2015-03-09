#pragma once

#include <lua.hpp>

#include <mye/core/Game.h>
#include "LuaModule.h"

#define MYE_GET_LUA_MODULE() (static_cast<mye::lua::LuaModule*>(mye::core::Game::GetSingleton().GetScriptModule()))
#define MYE_GET_LUA_STATE()  (MYE_GET_LUA_MODULE()->GetLuaState())

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