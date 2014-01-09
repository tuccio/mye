#include "LuaScript.h"
#include "LuaModule.h"
#include "LuaScriptCaller.h"

#include <lua.hpp>

using namespace mye::lua;
using namespace mye::core;

Script<LuaScriptCaller>::Script(mye::core::ScriptModule<LuaScriptCaller> &module,
								Type type) :
	m_module(module),
	m_type(type),
	m_ref(new LuaRegistryReference(module.GetLuaState()))
{
}

Script<LuaScriptCaller>::Script(mye::core::ScriptModule<LuaScriptCaller> &module,
								Type type,
								int index) :
	m_module(module),
	m_type(type),
	m_ref(new LuaRegistryReference(module.GetLuaState(), index))
{
}

Script<LuaScriptCaller>::~LuaScript(void)
{
}

std::shared_ptr<const mye::lua::LuaRegistryReference> Script<LuaScriptCaller>::GetReference(void) const
{
	return m_ref;
}

ScriptModule<mye::lua::LuaScriptCaller> Script<LuaScriptCaller>::GetModule(void) const
{
	return m_module;
}

Script<LuaScriptCaller>::Type Script<LuaScriptCaller>::GetType(void) const
{
	return m_type;
}

bool Script<LuaScriptCaller>::Run(void) const
{
	return LuaScriptCaller::Run(m_module, *this);;
}