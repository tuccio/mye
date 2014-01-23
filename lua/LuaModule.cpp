#include "LuaModule.h"

#include "CoreTypes.h"
#include "Math.h"
#include "Game.h"

#include "LuaScript.h"
#include "LuaScriptCaller.h"

#include <luabind/luabind.hpp>

#include <iostream>

using namespace mye::lua;
using namespace mye::core;
using namespace std;

LuaModule::LuaModule(void)
{
}


LuaModule::~LuaModule(void)
{
}

lua_State* LuaModule::GetLuaState(void)
{
	return m_L;
}

bool LuaModule::RunFile(const string &file)
{

	if (luaL_loadfile(m_L, file.c_str()) ||
		lua_pcall(m_L, 0, 0, 0))
	{
		m_lastError = lua_tostring(m_L, -1);
		return false;
	}

	return true;

}

bool LuaModule::RunString(const string &code)
{

	if (luaL_loadstring(m_L, code.c_str()) ||
		lua_pcall(m_L, 0, 0, 0))
	{
		m_lastError = lua_tostring(m_L, -1);
		return false;
	}

	return true;

}

string LuaModule::GetLastError(void) const
{
	return m_lastError;
}

void LuaModule::OpenAllLibraries(void)
{

	luaL_openlibs(m_L);
	luabind::open(m_L);

	BindCoreTypes(m_L);
	BindMath(m_L);
	BindGame(m_L);

}

bool LuaModule::Init(void)
{

	m_L = luaL_newstate();
	OpenAllLibraries();

	Game &game = Game::GetSingleton();

	luabind::globals(m_L)["Game"] = boost::ref(game);
	luabind::globals(m_L)["GameObjects"] = boost::ref(*game.GetGameObjectsModule());
	luabind::globals(m_L)["Script"] = boost::ref(*this);
	luabind::globals(m_L)["Input"] = boost::ref(*game.GetInputModule());
	luabind::globals(m_L)["Graphics"] = boost::ref(*game.GetGraphicsModule());

	return true;
}

void LuaModule::ShutDown(void)
{
	lua_close(m_L);
}

LuaScript LuaModule::LoadBehaviour(const std::string &filename)
{

	int top = lua_gettop(m_L);

	if (!luaL_loadfile(m_L, filename.c_str()))
	{

		// Create _ENV for sandbox

		lua_newtable(m_L);

		// Create its metatable
		lua_newtable(m_L);
		lua_getglobal(m_L, "_G");

		lua_setfield(m_L, -2, "__index");
		lua_setmetatable(m_L, -2);

		const char *upvalue = lua_setupvalue(m_L, -2, 1);

		if (upvalue != NULL)
		{

			assert(strncmp(upvalue, "_ENV", 4) == 0);

			lua_getupvalue(m_L, -1, 1);
			lua_insert(m_L, -2);

			if (!lua_pcall(m_L, 0, 0, 0))
			{
				LuaScript rvalue(*this, LuaScript::BEHAVIOUR, -1);
				lua_settop(m_L, top);
				return rvalue;
			}

		}

	}

	luaL_error(m_L, (std::string("Error while loading ") + filename).c_str());

	lua_settop(m_L, top);

	LuaScript rvalue(*this, LuaScript::BEHAVIOUR);
	return rvalue;

}

LuaScript LuaModule::LoadProcedure(const std::string &filename)
{

	int top = lua_gettop(m_L);

	if (!luaL_loadfile(m_L, filename.c_str()))
	{

		// Create _ENV for sandbox

		lua_newtable(m_L);

		// Create its metatable
		lua_newtable(m_L);
		lua_getglobal(m_L, "_G");

		lua_setfield(m_L, -2, "__index");
		lua_setmetatable(m_L, -2);

		const char *upvalue = lua_setupvalue(m_L, -2, 1);

		if (upvalue != NULL)
		{

			assert(strncmp(upvalue, "_ENV", 4) == 0);

			LuaScript rvalue(*this, LuaScript::PROCEDURE, -1);
			lua_settop(m_L, top);
			return rvalue;

		}

	}

	luaL_error(m_L, (std::string("Error while loading ") + filename).c_str());

	lua_settop(m_L, top);

	LuaScript rvalue(*this, LuaScript::PROCEDURE);
	return rvalue;

}