#include "BehaviourScript.h"

#include <lua.hpp>

using namespace mye::lua;

BehaviourScript::BehaviourScript(void)
{

}

BehaviourScript::BehaviourScript(LuaModule &luaModule,
								 const mye::core::String &name) :
	Script(luaModule, name)
{
}

BehaviourScript::~BehaviourScript(void)
{
}

bool BehaviourScript::LoadImpl(void)
{

	int top = lua_gettop(m_lua);
	bool loaded = false;

	if (!luaL_loadfile(m_lua, (m_scriptDirectory + m_name + ".lua").CString()))
	{

		// Create _ENV for sandbox

		lua_newtable(m_lua);

		// Create its metatable
		lua_newtable(m_lua);
		lua_getglobal(m_lua, "_G");

		lua_setfield(m_lua, -2, "__index");
		lua_setmetatable(m_lua, -2);

		const char *upvalue = lua_setupvalue(m_lua, -2, 1);

		if (upvalue != nullptr)
		{

			assert(strncmp(upvalue, "_ENV", 4) == 0);

			lua_getupvalue(m_lua, -1, 1);
			lua_insert(m_lua, -2);

			if (!lua_pcall(m_lua, 0, 0, 0))
			{
				m_registryReference = luaL_ref(m_lua, LUA_REGISTRYINDEX);
				loaded = true;
			}

		}

	}

	if (!loaded)
	{
		luaL_error(m_lua, ("Error while loading " + m_name).CString());
	}
	
	lua_settop(m_lua, top);
	return loaded;

}

void BehaviourScript::UnloadImpl(void)
{
	Clear();
}