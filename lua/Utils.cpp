#include "Utils.h"
#include "Types.h"

using namespace mye::lua;

int mye::lua::__mye_pushnil(lua_State *L)
{

	lua_pushnil(L);
	return 1;

}

int mye::lua::__mye_donothing(lua_State *L)
{

	return 0;

}