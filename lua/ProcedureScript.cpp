#include "ProcedureScript.h"
#include "Utils.h"
#include "LuaModule.h"

#include <lua.hpp>

using namespace mye::lua;

ProcedureScript::ProcedureScript(void)
{

}

ProcedureScript::ProcedureScript(const mye::core::String &name) :
	Script(MYE_GET_LUA_MODULE(), name)
{
}

ProcedureScript::~ProcedureScript(void)
{
}

bool ProcedureScript::LoadImpl(void)
{

	bool loaded = false;

	LuaModule * module = static_cast<LuaModule*>(m_owner);

	lua_State * L = module->GetLuaState();

	LuaStackCleaner stackCleaner(L);

	mye::core::String dir;

	if (m_params.Contains("directory"))
	{
		dir = m_params.GetString("directory");
	}
	else
	{
		dir = module->GetScriptDirectory();
	}

	mye::core::String fullpath = dir + m_name + MYE_LUA_SCRIPT_EXTENSION;

	if (!luaL_loadfile(L, fullpath.CString()))
	{
		m_script = luapp11::FromStack(L, -1);
		loaded = true;
	}
	else
	{
		m_error = lua_tostring(L, -1);
	}

	return loaded;

}

void ProcedureScript::UnloadImpl(void)
{
	Clear();
}