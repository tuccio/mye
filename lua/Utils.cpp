#include "Utils.h"
#include "Types.h"

using namespace mye::lua;

int mye::lua::__mye_pushnil(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}

int mye::lua::__mye_noop(lua_State *L)
{
	return 0;
}

void mye::lua::RegisterUtils(lua_State *L)
{

	lua_pushstring(L, MYE_LUA_NOOP);
	lua_pushcfunction(L, __mye_noop);

	lua_settable(L, LUA_REGISTRYINDEX);

	lua_pushstring(L, MYE_LUA_PUSHNIL);
	lua_pushcfunction(L, __mye_pushnil);

	lua_settable(L, LUA_REGISTRYINDEX);	

}