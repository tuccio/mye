#include "LuaScript.h"
#include "LuaModule.h"
#include "LuaScriptCaller.h"

#include <lua.hpp>

using namespace mye::lua;

LuaScript::LuaScript(LuaModule &module,
					 Type type) :
	m_module(module),
	m_type(type),
	m_ref(new LuaRegistryReference(module.GetLuaState()))
{
}

LuaScript::LuaScript(LuaModule &module,
					 Type type,
					 int index) :
	m_module(module),
	m_type(type),
	m_ref(new LuaRegistryReference(module.GetLuaState(), index))
{
}

LuaScript::~LuaScript(void)
{
}

LuaScript::Reference LuaScript::GetReference(void) const
{
	return m_ref;
}

/*
LuaModule LuaScript::GetModule(void) const
{
	return m_module;
}*/

LuaScript::Type LuaScript::GetType(void) const
{
	return m_type;
}

bool LuaScript::Run(void) const
{
	
	lua_State *L = m_module.GetLuaState();
	int top = lua_gettop(L);

	if (m_type == LuaScript::Type::PROCEDURE)
	{

		int ref = *GetReference();

		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

		if (!lua_pcall(L, 0, 0, 0))
		{
			lua_settop(L, top);
			return true;
		}

	}

	std::string errorMessage = lua_tostring(L, -1);

	mye::core::Game::GetSingletonPointer()->RuntimeError(errorMessage);

	luaL_error(L, errorMessage.c_str());
	lua_settop(L, top);

	return false;

}