#include "Script.h"
#include "LuaModule.h"

using namespace mye::lua;

Script::Script(void) :
	Resource(nullptr, "", nullptr),
	m_lua(nullptr),
	m_registryReference(LUA_NOREF)
{
}

Script::Script(LuaModule &luaModule, const mye::core::String &name) :
	Resource(&luaModule, name, nullptr),
	m_lua(luaModule.GetLuaState()),
	m_registryReference(LUA_NOREF),
	m_scriptDirectory(luaModule.GetScriptDirectory())
{
}


Script::~Script(void)
{
}

size_t Script::CalculateSizeImpl(void)
{
	return 1; // TODO: Calcolare rimensione script
}

void Script::Clear(void)
{
	luaL_unref(m_lua, LUA_REGISTRYINDEX, m_registryReference);
	m_registryReference = LUA_NOREF;
}