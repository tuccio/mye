#include "Utils.h"

using namespace mye::lua;

LuaStackCleaner::LuaStackCleaner(lua_State *lua) :
	m_lua(lua)
{
	m_top = lua_gettop(m_lua);
}

LuaStackCleaner::~LuaStackCleaner(void)
{
	lua_settop(m_lua, m_top);
}