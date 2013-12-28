#include "LuaEnvironment.h"

#include "Vector3.h"
#include "Game.h"

#include <luabind/luabind.hpp>

using namespace mye::lua;
using namespace std;

LuaEnvironment::LuaEnvironment(void)
{
	_L = luaL_newstate();
	OpenAllLibraries();
	_game = NULL;
}


LuaEnvironment::~LuaEnvironment(void)
{
	lua_close(_L);
}

lua_State* LuaEnvironment::GetLuaState(void)
{
	return _L;
}

bool LuaEnvironment::RunFile(const string &file)
{

	if (luaL_loadfile(_L, file.c_str()) ||
		lua_pcall(_L, 0, 0, 0))
	{
		_lastError = lua_tostring(_L, -1);
		return false;
	}

	return true;

}

bool LuaEnvironment::RunString(const string &code)
{

	if (luaL_loadstring(_L, code.c_str()) ||
		lua_pcall(_L, 0, 0, 0))
	{
		_lastError = lua_tostring(_L, -1);
		return false;
	}

	return true;

}

string LuaEnvironment::GetLastError(void) const
{
	return _lastError;
}

void LuaEnvironment::SetGameInstance(mye::core::Game &game)
{

	_game = &game;

	luabind::globals(_L)["Game"] = boost::ref(game);
	luabind::globals(_L)["GameObjects"] = boost::ref(*game.GetGameObjectsModule());

}

void LuaEnvironment::OpenAllLibraries(void)
{

	luaL_openlibs(_L);
	luabind::open(_L);

	BindVector3(_L);
	BindGame(_L);

}