#pragma once

#include "Script.h"
#include "Types.h"

#include <mye/core/Game.h>

#include <mye/win/Window.h>

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

					def("LoadClass", &LuaModule::LoadClass).
					def("LoadProcedure", &LuaModule::LoadProcedure)

			];

			module(L)
			[

				class_<LuaScript>(MYE_LUA_SCRIPT).

					def("Run", &LuaScript::Run)

			];

		}

	}

}