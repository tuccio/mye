#include "LuaModule.h"

#include "CoreTypes.h"
#include "Converters.h"
#include "Math.h"
#include "Game.h"

#include <luabind/luabind.hpp>

#include <mye/core/String.h>

#include <iostream>

using namespace mye::lua;
using namespace mye::core;

LuaModule::LuaModule(const mye::core::String &folder) :
	m_scriptDirectory(folder),
	mye::core::ResourceManager("LuaScript")
{
}


LuaModule::~LuaModule(void)
{
}

lua_State* LuaModule::GetLuaState(void)
{
	return m_lua;
}

const mye::core::String& LuaModule::GetScriptDirectory(void) const
{
	return m_scriptDirectory;
}

bool LuaModule::RunFile(const String &file)
{

	if (luaL_loadfile(m_lua, file.CString()) ||
		lua_pcall(m_lua, 0, 0, 0))
	{
		m_lastError = lua_tostring(m_lua, -1);
		return false;
	}

	return true;

}

bool LuaModule::RunString(const String &code)
{

	if (luaL_loadstring(m_lua, code.CString()) ||
		lua_pcall(m_lua, 0, 0, 0))
	{
		m_lastError = lua_tostring(m_lua, -1);
		return false;
	}

	return true;

}

String LuaModule::GetLastError(void) const
{
	return m_lastError;
}

void LuaModule::OpenAllLibraries(void)
{

	luaL_openlibs(m_lua);
	luabind::open(m_lua);

	BindCoreTypes(m_lua);
	BindMath(m_lua);
	BindGame(m_lua);

}

bool LuaModule::Init(void)
{

	m_lua = luaL_newstate();
	OpenAllLibraries();

	Game &game = Game::GetSingleton();

	luabind::globals(m_lua)["Game"]        = boost::ref(game);
	luabind::globals(m_lua)["GameObjects"] = boost::ref(*game.GetGameObjectsModule());
	luabind::globals(m_lua)["Script"]      = boost::ref(*this);
	luabind::globals(m_lua)["Input"]       = boost::ref(*game.GetInputModule());
	luabind::globals(m_lua)["Graphics"]    = boost::ref(*game.GetGraphicsModule());

	return true;

}

void LuaModule::ShutDown(void)
{
	lua_close(m_lua);
}

BehaviourScriptPointer LuaModule::LoadBehaviour(const mye::core::String &name)
{

	Resource::ParametersList params;
	params["type"] = "behaviour";

	BehaviourScriptPointer script = CreateResource<BehaviourScript>(name, nullptr, &params);

	if (script)
	{
		script->Load();
	}

	return script;

}

ProcedureScriptPointer LuaModule::LoadProcedure(const mye::core::String &name)
{

	ProcedureScriptPointer script = CreateResource<ProcedureScript>(name, nullptr, nullptr);

	if (script)
	{
		script->Load();
	}

	return script;

}

/*
LuaScript LuaModule::LoadProcedure(const mye::core::String &filename)
{

	int top = lua_gettop(m_lua);

	if (!luaL_loadfile(m_lua, filename.CString()))
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

			LuaScript rvalue(*this, LuaScript::PROCEDURE, -1);
			lua_settop(m_lua, top);
			return rvalue;

		}

	}

	luaL_error(m_lua, ("Error while loading " + filename).CString());

	lua_settop(m_lua, top);

	LuaScript rvalue(*this, LuaScript::PROCEDURE);
	return rvalue;

}*/

Script* LuaModule::CreateImpl(const String &name,
								ManualResourceLoader *manual,
								Resource::ParametersList *params)
{

	bool procedure = true;

	if (params &&
		(*params)["type"] == "behaviour")
	{
		procedure = false;
	}

	return (procedure ?
		static_cast<Script*>(new ProcedureScript(*this, name)) :
		static_cast<Script*>(new BehaviourScript(*this, name)));

}

void LuaModule::FreeImpl(Resource* resource)
{
	static_cast<Script*>(resource)->Free();
}