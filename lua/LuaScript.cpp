#include "LuaScript.h"
#include "LuaModule.h"
#include "LuaScriptCaller.h"

#include <lua.hpp>

using namespace mye::lua;
using namespace mye::core;

Script<LuaScriptCaller>::Script(mye::core::ScriptModule<LuaScriptCaller> &module,
								Type type) :
	_module(module),
	_type(type),
	_ref(new LuaRegistryReference(module.GetLuaState()))
{
}

Script<LuaScriptCaller>::Script(mye::core::ScriptModule<LuaScriptCaller> &module,
								Type type,
								int index) :
	_module(module),
	_type(type),
	_ref(new LuaRegistryReference(module.GetLuaState(), index))
{
}

Script<LuaScriptCaller>::~LuaScript(void)
{
}

std::shared_ptr<const mye::lua::LuaRegistryReference> Script<LuaScriptCaller>::GetReference(void) const
{
	return _ref;
}

ScriptModule<mye::lua::LuaScriptCaller> Script<LuaScriptCaller>::GetModule(void) const
{
	return _module;
}

Script<LuaScriptCaller>::Type Script<LuaScriptCaller>::GetType(void) const
{
	return _type;
}

bool Script<LuaScriptCaller>::Run(void) const
{
	return LuaScriptCaller::Run(_module, *this);;
}