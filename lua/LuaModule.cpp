#include "LuaModule.h"

#include "Converters.h"
#include "Alignment.h"
#include "Math.h"
#include "Scene.h"
#include "Physics.h"
#include "Game.h"
#include "Resource.h"

#include <mye/core/String.h>
#include <mye/core/ResourceTypeManager.h>

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

	BindResources(m_lua);
	BindMath(m_lua);
	BindScene(m_lua);
	BindPhysics(m_lua);
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
	luabind::globals(m_lua)["Physics"]     = boost::ref(*game.GetPhysicsModule());
	luabind::globals(m_lua)["Scene"]       = boost::ref(*game.GetSceneModule());

	luabind::globals(m_lua)["Time"]                = luabind::newtable(m_lua);
	luabind::globals(m_lua)["ResourceTypeManager"] = boost::ref(ResourceTypeManager::GetSingleton());

	return true;

}

void LuaModule::ShutDown(void)
{
	lua_close(m_lua);
}

void LuaModule::Preupdate(FloatSeconds dt)
{

	luabind::globals(m_lua)["Time"]["delta"] = luabind::object(m_lua, (float) dt);;

}

BehaviourScriptPointer LuaModule::LoadBehaviour(const mye::core::String &name)
{

	Parameters params;
	params["type"] = "behaviour";

	BehaviourScriptPointer script = CreateResource<BehaviourScript>(name, nullptr, params);

	if (script)
	{
		script->Load();
	}

	return script;

}

ProcedureScriptPointer LuaModule::LoadProcedure(const mye::core::String &name)
{

	Parameters params;
	params["type"] = "procedure";

	ProcedureScriptPointer script = CreateResource<ProcedureScript>(name, nullptr, params);

	if (script)
	{
		script->Load();
	}

	return script;

}

ScriptResourceLoaderPointer LuaModule::LoadScriptResourceLoader(const mye::core::String &name)
{

	Parameters params;
	params["type"] = "resource loader";

	ScriptResourceLoaderPointer script = CreateResource<ScriptResourceLoader>(name, nullptr, params);

	if (script)
	{
		script->Load();
	}

	return script;

}

Script* LuaModule::CreateImpl(const String &name,
							  ManualResourceLoader *manual,
							  const Parameters &params)
{

	Script *script = nullptr;

	if (params.Contains("type"))
	{

		String type = params.GetString("type");

		if (type == "behaviour")
		{
			script = new BehaviourScript(*this, name);
		}
		else if (type == "resource loader")
		{
			script = new ScriptResourceLoader(*this, name);
		}
		else if (type == "procedure")
		{
			script = new ProcedureScript(*this, name);
		}

	}
	else
	{
		script = new ProcedureScript(*this, name);
	}

	return script;

}

void LuaModule::FreeImpl(Resource* resource)
{
	static_cast<Script*>(resource)->Free();
}