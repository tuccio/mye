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

	// Create the table

	lua_pushstring(L, MYE_LUA_REGKEY);
	lua_newtable(L);

	// Fill the table

	lua_pushcfunction(L, __mye_noop);
	lua_setfield(L, -2, MYE_LUA_NOOP);

	lua_pushcfunction(L, __mye_pushnil);
	lua_setfield(L, -2, MYE_LUA_PUSHNIL);

	// Push the table to the registry
	
	lua_settable(L, LUA_REGISTRYINDEX);

}

luabind::object mye::lua::RegistryEntry(lua_State *L)
{

	return luabind::registry(L)[MYE_LUA_REGKEY];

}

void* mye::lua::UserdataMetatableCheck(lua_State *L, int index, const char *type)
{

	void *p = lua_touserdata(L, index);

	if (p)
	{

		if (lua_getmetatable(L, index))
		{

			lua_pushstring(L, MYE_LUA_REGKEY);
			lua_gettable(L, LUA_REGISTRYINDEX);
			
			lua_getfield(L, -1, MYE_LUA_GAMEOBJECT);

			if (!lua_rawequal(L, -1, -3))
			{
				p = NULL;
			}

			lua_pop(L, 3);

		}

	}

	return p;

}

std::string mye::lua::GetMYEType(lua_State *L, int index)
{

	std::string type;
	
	if (lua_getmetatable(L, index))
	{

		lua_getfield(L, -1, "__myetype");

		if (lua_type(L, -1) == LUA_TSTRING)
		{
			type = lua_tostring(L, -1);
		}

		lua_pop(L, 2);

	}

	return type;

}

void mye::lua::SetMYEType(const luabind::object &o, const std::string &type)
{

	luabind::object mt = luabind::getmetatable(o);

	if (mt)
	{
		mt["__myetype"] = type;
	}

}