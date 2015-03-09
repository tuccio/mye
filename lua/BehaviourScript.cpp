#include "BehaviourScript.h"
#include "Utils.h"

#include <mye/core/Game.h>
#include <mye/core/Utils.h>

#include <lua.hpp>

#include "LuaModule.h"

using namespace mye::lua;

BehaviourScript::BehaviourScript(void)
{

}

BehaviourScript::BehaviourScript(const mye::core::String &name) :
	Script(MYE_GET_LUA_MODULE(), name)
{
}

BehaviourScript::~BehaviourScript(void)
{
}

bool BehaviourScript::LoadImpl(void)
{

	mye::core::String dir;

	LuaModule * module = static_cast<LuaModule*>(m_owner);

	if (m_params.Contains("directory"))
	{
		dir = m_params.GetString("directory");
	}
	else
	{
		dir = module->GetScriptDirectory();
	}
	mye::core::String fullpath;

	if (!dir.IsEmpty())
	{
		fullpath = dir + '/' + m_name + MYE_LUA_SCRIPT_EXTENSION;
	}

	lua_State * L = module->GetLuaState();
	return m_behaviour.Create(L, fullpath.CString());

}

void BehaviourScript::UnloadImpl(void)
{
	Clear();
}