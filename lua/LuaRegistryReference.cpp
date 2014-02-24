#include "LuaRegistryReference.h"
#include "Converters.h"

using namespace mye::lua;

LuaRegistryReference::LuaRegistryReference(lua_State *L)
{
	this->L = L;
	ref = LUA_NOREF;
}

LuaRegistryReference::LuaRegistryReference(lua_State *L,
					 int index)
{
	this->L = L;
	lua_pushvalue(L, index);
	ref = luaL_ref(L, LUA_REGISTRYINDEX);
}

LuaRegistryReference::~LuaRegistryReference(void)
{
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

LuaRegistryReference::operator int () const
{
	return ref;
}