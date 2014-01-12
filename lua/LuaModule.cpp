#include "LuaModule.h"

#include "Math.h"
#include "Game.h"

#include "LuaScript.h"
#include "LuaScriptCaller.h"

#include <luabind/luabind.hpp>

#include <iostream>

using namespace mye::lua;
using namespace mye::core;
using namespace std;

ScriptModule<LuaScriptCaller>::ScriptModule(void)
{
}


ScriptModule<LuaScriptCaller>::~ScriptModule(void)
{
}

lua_State* ScriptModule<LuaScriptCaller>::GetLuaState(void)
{
	return _L;
}

bool ScriptModule<LuaScriptCaller>::RunFile(const string &file)
{

	if (luaL_loadfile(_L, file.c_str()) ||
		lua_pcall(_L, 0, 0, 0))
	{
		_lastError = lua_tostring(_L, -1);
		return false;
	}

	return true;

}

bool ScriptModule<LuaScriptCaller>::RunString(const string &code)
{

	if (luaL_loadstring(_L, code.c_str()) ||
		lua_pcall(_L, 0, 0, 0))
	{
		_lastError = lua_tostring(_L, -1);
		return false;
	}

	return true;

}

string ScriptModule<LuaScriptCaller>::GetLastError(void) const
{
	return _lastError;
}

void ScriptModule<LuaScriptCaller>::OpenAllLibraries(void)
{

	luaL_openlibs(_L);
	luabind::open(_L);

	BindMath(_L);
	BindGame(_L);

}

bool ScriptModule<LuaScriptCaller>::Init(void)
{

	_L = luaL_newstate();
	OpenAllLibraries();

	Game &game = Game::GetSingleton();

	luabind::globals(_L)["Game"] = boost::ref(game);
	luabind::globals(_L)["GameObjects"] = boost::ref(*game.GetGameObjectsModule());
	luabind::globals(_L)["Script"] = boost::ref(*this);

	return true;
}

void ScriptModule<LuaScriptCaller>::ShutDown(void)
{
	lua_close(_L);
}

mye::core::Script<LuaScriptCaller> ScriptModule<LuaScriptCaller>::LoadClass(const std::string &filename)
{
	return LuaScriptCaller::LoadClass(*this, filename);
}

mye::core::Script<LuaScriptCaller> ScriptModule<LuaScriptCaller>::LoadProcedure(const std::string &filename)
{
	return LuaScriptCaller::LoadProcedure(*this, filename);
}