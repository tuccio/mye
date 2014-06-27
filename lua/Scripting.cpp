#pragma once

#include "Scripting.h"
#include "Types.h"
#include "Converters.h"
#include "Alignment.h"

#include <mye/core/Game.h>
#include <mye/core/Resource.h>

#include <luabind/luabind.hpp>

#include "Game.h"
#include "LuaModule.h"
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
					def("LoadProcedure", &LuaModule::LoadProcedure).
					def("LoadScriptResourceLoader", &LuaModule::LoadScriptResourceLoader),

				class_<Script, mye::core::Resource>(MYE_LUA_SCRIPT),

				class_<BehaviourScript, Script>(MYE_LUA_BEHAVIOURSCRIPT),

				class_<ProcedureScript, Script>(MYE_LUA_PROCEDURESCRIPT).

					def("Run", &ProcedureScript::Run)				

			];

		}

	}

}