#include "LuaModule.h"

#include "Converters.h"
#include "Math.h"
#include "Scene.h"
#include "Physics.h"
#include "Game.h"
#include "Resource.h"

#include "MetaMethodsOverload.h"

#include <mye/core/String.h>
#include <mye/core/ResourceTypeManager.h>

#include <iostream>

using namespace mye::lua;
using namespace mye::core;

LuaModule::LuaModule(const mye::core::String &folder) :
m_scriptDirectory(folder),
mye::core::ResourceManager("Script")
{
}


LuaModule::~LuaModule(void)
{
}

lua_State * LuaModule::GetLuaState(void)
{
	return m_state.GetState();
}

const mye::core::String & LuaModule::GetScriptDirectory(void) const
{
	return m_scriptDirectory;
}

bool LuaModule::RunFile(const String &file)
{
	return m_state.RunFile(file.CString());
}

bool LuaModule::RunString(const String &code)
{
	return m_state.RunCode(code.CString());
}

String LuaModule::GetLastError(void) const
{
	return m_lastError;
}

void LuaModule::OpenAllLibraries(void)
{

	m_state.OpenLibraries();

	BindResources(m_state);
	BindMath(m_state);
	BindScene(m_state);
	BindPhysics(m_state);
	BindGame(m_state);

}

bool LuaModule::Init(void)
{

	m_state.Create();
	OpenAllLibraries();

	/*OverloadLuabindMetamethod(m_lua, "__index", &IndexOverload);
	OverloadLuabindMetamethod(m_lua, "__newindex", &NewIndexOverload);*/

	Game * game = Game::GetSingletonPointer();

	auto globals = m_state.GetGlobalTable();;

	globals["Game"]        = game;
	globals["GameObjects"] = game->GetGameObjectsModule();
	globals["Script"]      = this;
	globals["Input"]       = game->GetInputModule();
	globals["Graphics"]    = game->GetGraphicsModule();
	globals["Physics"]     = game->GetPhysicsModule();
	globals["Scene"]       = game->GetSceneModule();

	globals["Time"]        = luapp11::CreateTable(m_state);

	globals["ResourceTypeManager"] = ResourceTypeManager::GetSingletonPointer();

	m_state.RunCode("math.randomseed(os.time())");

	return true;

}

void LuaModule::ShutDown(void)
{
	m_state.Destroy();
}

void LuaModule::Preupdate(FloatSeconds dt)
{

	//luabind::globals(m_lua)["Time"]["delta"] = luabind::object(m_lua, (float) dt);

	m_state.GetGlobalTable()["Time"]["delta"] = (float) dt;

	for (auto msg : m_messages)
	{
		
		switch (msg.message)
		{

		case SCRIPT_MESSAGE_KEYBOARD_PRESSED:

			{

				luapp11::Object o(m_state, msg.hObj);
				KeyboardVK key = static_cast<KeyboardVK>(msg.code);

				try
				{
					o.Call<void>("OnKeyboardKeyPress", std::forward<KeyboardVK>(key));
				}
				catch (...) { }

			}

			break;

		case SCRIPT_MESSAGE_KEYBOARD_RELEASED:

			{

				luapp11::Object o(m_state, msg.hObj);
				KeyboardVK key = static_cast<KeyboardVK>(msg.code);

				try
				{
					o.Call<void>("OnKeyboardKeyRelease", std::forward<KeyboardVK>(key), std::forward<float>(msg.data.f));
				}
				catch (...) { }

			}

			break;

		case SCRIPT_MESSAGE_KEYBOARD_HELD:

		{

			luapp11::Object o(m_state, msg.hObj);
			KeyboardVK key = static_cast<KeyboardVK>(msg.code);

			try
			{
				o.Call<void>("OnKeyboardKeyHold", std::forward<KeyboardVK>(key), std::forward<float>(msg.data.f));
			}
			catch (...) { }

		}

			break;

		case SCRIPT_MESSAGE_MOUSE_PRESSED:

		{

			luapp11::Object o(m_state, msg.hObj);
			MouseVK key = static_cast<MouseVK>(msg.code);

			try
			{
				o.Call<void>("OnMouseKeyPress", std::forward<MouseVK>(key));
			}
			catch (...) { }

		}

			break;

		case SCRIPT_MESSAGE_MOUSE_RELEASED:

		{

			luapp11::Object o(m_state, msg.hObj);
			MouseVK key = static_cast<MouseVK>(msg.code);

			try
			{
				o.Call<void>("OnMouseKeyRelease", std::forward<MouseVK>(key), std::forward<float>(msg.data.f));
			}
			catch (...) { }

		}

			break;

		case SCRIPT_MESSAGE_MOUSE_HELD:

		{

			luapp11::Object o(m_state, msg.hObj);
			MouseVK key = static_cast<MouseVK>(msg.code);

			try
			{
				o.Call<void>("OnMouseKeyHold", std::forward<MouseVK>(key), std::forward<float>(msg.data.f));
			}
			catch (...) { }

		}

			break;

		case SCRIPT_MESSAGE_MOUSE_MOVED:

		{

			luapp11::Object o(m_state, msg.hObj);
			MouseVK key = static_cast<MouseVK>(msg.code);

			const mye::math::Vector2 from(msg.data.f4[0], msg.data.f4[1]);
			const mye::math::Vector2 to(msg.data.f4[2], msg.data.f4[3]);

			try
			{
				o.Call<void>("OnMouseMove", from, to);
			}
			catch (...) { }

		}

			break;

		}

	}

	m_messages.clear();

}

void LuaModule::Init(GameObjectHandle hObj)
{
	luapp11::Object o(m_state, hObj);

	try
	{
		o.Call<void>("Init");
	}
	catch (...) { }
	
}

void LuaModule::Finalize(GameObjectHandle hObj)
{

	luapp11::Object o(m_state, hObj);

	try
	{
		o.Call<void>("Finalize");
	}
	catch (...) { }
	
}

void LuaModule::Update(GameObjectsModule::Iterator it)
{
	GameObjectHandle hObj = *it;
	luapp11::Object o(m_state, hObj);

	try
	{
		o.Call<void>("Update");
	}
	catch (...) { }
}

BehaviourScriptPointer LuaModule::LoadBehaviour(const String &name)
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

ScriptResourceLoaderPointer LuaModule::LoadScriptResourceLoader(const String &name)
{

	Parameters params;
	params["type"] = "resource loader";

	ScriptResourceLoaderPointer script = CreateResource<ScriptResourceLoader>(name, nullptr, params);

	if (script)
	{
		script->Load(false);
	}


	return script;

}

Script * LuaModule::CreateImpl(const String &name,
							   ManualResourceLoader *manual,
							   const Parameters &params)
{

	Script * script = nullptr;

	if (params.Contains("type"))
	{

		String type = params.GetString("type");

		if (type == "behaviour")
		{
			script = new BehaviourScript(name);
		}
		else if (type == "resource loader")
		{
			script = new ScriptResourceLoader(name);
		}
		else if (type == "procedure")
		{
			script = new ProcedureScript(name);
		}

	}
	else
	{
		script = new ProcedureScript(name);
	}

	return script;

}

void LuaModule::FreeImpl(Resource* resource)
{
	static_cast<Script*>(resource)->Free();
}