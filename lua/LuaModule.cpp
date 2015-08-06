#include "LuaModule.h"

#include "Converters.h"
#include "Math.h"
#include "Scene.h"
#include "Physics.h"
#include "Game.h"
#include "Resource.h"

#include "MetaMethodsOverload.h"

#include <mye/core/EventManager.h>
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

	globals["r"] = game->GetGraphicsModule()->GetRendererConfiguration();

	return true;

}

void LuaModule::Shutdown(void)
{
	m_state.Destroy();
}

void LuaModule::Preupdate(FloatSeconds dt)
{

	m_state.GetGlobalTable()["Time"]["delta"] = (float) dt;

}

void LuaModule::Init(GameObject * object)
{

	luapp11::Object o(m_state, object->GetHandle());

	auto f = o["Init"];

	if (f.GetLuaType() != LUA_TNIL)
	{
		o.Call<void>("Init");
	}
	
}

void LuaModule::Finalize(GameObject * object)
{

	luapp11::Object o(m_state, object->GetHandle());

	auto f = o["Finalize"];

	if (f.GetLuaType() != LUA_TNIL)
	{
		o.Call<void>("Finalize");
	}	
	
}

void LuaModule::Update(void)
{

	GameObjectsModule * gom = Game::GetSingleton().GetGameObjectsModule();

	for (auto hObj : *gom)
	{

		luapp11::Object o(m_state, hObj);

		auto f = o["Update"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("Update");
		}

	}
	
}

BehaviourScriptPointer LuaModule::LoadBehaviour(const String & name)
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

ProcedureScriptPointer LuaModule::LoadProcedure(const mye::core::String & name)
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

ScriptResourceLoaderPointer LuaModule::LoadScriptResourceLoader(const String & name)
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

void LuaModule::OnEvent(mye::core::GameObject * object, const IEvent * e)
{

	switch (e->event)
	{

	case EventType::KEYBOARD_KEY_PRESS:

	{

		const KeyboardEventKeyPress * kEvent = static_cast<const KeyboardEventKeyPress *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnKeyboardKeyPress"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnKeyboardKeyPress", kEvent->key);
		}

	}

		break;

	case EventType::KEYBOARD_KEY_RELEASE:

	{

		const KeyboardEventKeyRelease * kEvent = static_cast<const KeyboardEventKeyRelease *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnKeyboardKeyRelease"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnKeyboardKeyRelease", kEvent->key, (float) kEvent->duration);
		}

	}

		break;

	case EventType::KEYBOARD_KEY_HOLD:

	{

		const KeyboardEventKeyHold * kEvent = static_cast<const KeyboardEventKeyHold *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnKeyboardKeyHold"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnKeyboardKeyHold", kEvent->key, (float) kEvent->duration);
		}

	}

		break;

	case EventType::MOUSE_KEY_PRESS:

	{

		const MouseEventKeyPress * kEvent = static_cast<const MouseEventKeyPress *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnMouseKeyPress"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnMouseKeyPress", kEvent->key);
		}

	}

		break;

	case EventType::MOUSE_KEY_RELEASE:

	{

		const MouseEventKeyRelease * kEvent = static_cast<const MouseEventKeyRelease *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnMouseKeyRelease"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnMouseKeyRelease", kEvent->key, (float) kEvent->duration);
		}

	}

		break;

	case EventType::MOUSE_KEY_HOLD:

	{

		const MouseEventKeyHold * kEvent = static_cast<const MouseEventKeyHold *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnMouseKeyRelease"];

		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnMouseKeyRelease", kEvent->key, (float) kEvent->duration);
		}

	}

		break;

	case EventType::MOUSE_MOVE:

	{

		const MouseEventMove * kEvent = static_cast<const MouseEventMove *>(e);

		luapp11::Object o(m_state, object->GetHandle());

		auto f = o["OnMouseMove"];
		
		if (f.GetLuaType() != LUA_TNIL)
		{
			o.Call<void>("OnMouseMove", kEvent->from, kEvent->to);
		}

	}

		break;

	}

}