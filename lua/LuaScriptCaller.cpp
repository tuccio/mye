/*
#include <mye/core/Game.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <cassert>

#include "LuaScriptCaller.h"
#include "LuaScript.h"
#include "LuaModule.h"

namespace mye
{

	namespace lua
	{

		using namespace mye::core;

		Script<LuaScriptCaller> LuaScriptCaller::LoadClass(
			ScriptModule<LuaScriptCaller> &module,
			const mye::core::String &filename)
		{

			lua_State *_L = module.GetLuaState();

			int top = lua_gettop(_L);

			if (!luaL_loadfile(_L, filename.c_str()))
			{

				// Create _ENV for sandbox

				lua_newtable(_L);

				// Create its metatable
				lua_newtable(_L);
				lua_getglobal(_L, "_G");

				lua_setfield(_L, -2, "__index");
				lua_setmetatable(_L, -2);

				const char *upvalue = lua_setupvalue(_L, -2, 1);

				if (upvalue != nullptr)
				{

					assert(strncmp(upvalue, "_ENV", 4) == 0);

					lua_getupvalue(_L, -1, 1);
					lua_insert(_L, -2);

					if (!lua_pcall(_L, 0, 0, 0))
					{
						LuaScript rvalue(module, LuaScript::BEHAVIOUR, -1);
						lua_settop(_L, top);
						return rvalue;
					}

				}

			}

			luaL_error(_L, (mye::core::String("Error while loading ") + filename).c_str());

			lua_settop(_L, top);

			LuaScript rvalue(module, LuaScript::BEHAVIOUR);
			return rvalue;

		}

		Script<LuaScriptCaller> LuaScriptCaller::LoadProcedure(
			ScriptModule<LuaScriptCaller> &module,
			const mye::core::String &filename)
		{

			lua_State *L = module.GetLuaState();

			int top = lua_gettop(L);

			if (!luaL_loadfile(L, filename.c_str()))
			{

				// Create _ENV for sandbox

				lua_newtable(L);

				// Create its metatable
				lua_newtable(L);
				lua_getglobal(L, "_G");

				lua_setfield(L, -2, "__index");
				lua_setmetatable(L, -2);

				const char *upvalue = lua_setupvalue(L, -2, 1);

				if (upvalue != nullptr)
				{

					assert(strncmp(upvalue, "_ENV", 4) == 0);

					LuaScript rvalue(module, LuaScript::PROCEDURE, -1);
					lua_settop(L, top);
					return rvalue;

				}

			}

			luaL_error(L, (mye::core::String("Error while loading ") + filename).c_str());

			lua_settop(L, top);

			LuaScript rvalue(module, LuaScript::PROCEDURE);
			return rvalue;

		}

		bool LuaScriptCaller::Run(ScriptModule<LuaScriptCaller> &module,
			const Script<LuaScriptCaller> &script)
		{

			lua_State *L = module.GetLuaState();
			int top = lua_gettop(L);

			if (script.GetType() == Script<LuaScriptCaller>::Type::PROCEDURE)
			{

				int ref = *script.GetReference();

				lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

				if (!lua_pcall(L, 0, 0, 0))
				{
					lua_settop(L, top);
					return true;
				}

			}

			mye::core::String errorMessage = lua_tostring(L, -1);

			Game *game = Game::GetSingletonPointer();

			if (game)
			{
				game->RuntimeError(mye::core::String("Lua script error: ") + errorMessage);
			}

			luaL_error(L, errorMessage.c_str());
			lua_settop(L, top);

			return false;

		}

	}

}
*/
