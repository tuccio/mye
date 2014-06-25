#include "ProcedureScript.h"
#include "Utils.h"

#include <lua.hpp>

using namespace mye::lua;

ProcedureScript::ProcedureScript(void)
{

}

ProcedureScript::ProcedureScript(LuaModule &luaModule,
								 const mye::core::String &name) :
	Script(luaModule, name)
{
}

ProcedureScript::~ProcedureScript(void)
{
}

bool ProcedureScript::LoadImpl(void)
{

	LuaStackCleaner stackCleaner(m_lua);

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

			m_registryReference = luaL_ref(m_lua, LUA_REGISTRYINDEX);
			return true;

		}

	}

	m_error = lua_tostring(m_lua, -1);

	return false;

}

void ProcedureScript::UnloadImpl(void)
{
	Clear();
}