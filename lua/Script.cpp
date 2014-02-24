#pragma once

#include "Script.h"
#include "Types.h"
#include "Converters.h"

#include <mye/core/Game.h>
#include <mye/core/ScriptComponent.h>

#include <luabind/luabind.hpp>

#include "Game.h"
#include "LuaModule.h"
#include "LuaScript.h"
#include "LuaScriptCaller.h"
#include "GameObjectHandle.h"

using namespace luabind;
using namespace mye::core;
using namespace mye::lua;

namespace mye
{

	namespace lua
	{

		void BindScripts(lua_State *L)
		{

			module(L)
			[

				class_<LuaModule>(MYE_LUA_LUAMODULE).

					def("LoadBehaviour", &LuaModule::LoadBehaviour).
					def("LoadProcedure", &LuaModule::LoadProcedure),

				class_<LuaScript>(MYE_LUA_SCRIPT).

					def("Run", &LuaScript::Run),

				class_<ScriptComponent, Component>(MYE_LUA_SCRIPT_COMPONENT).

					def(constructor<LuaScript>())

			];

		}

	}

}