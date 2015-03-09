#pragma once

#include "Scripting.h"
#include "Types.h"
#include "Converters.h"

#include <mye/core/Game.h>
#include <mye/core/Resource.h>
#include <mye/core/ResourceTypeManager.h>

#include "Game.h"
#include "LuaModule.h"
#include "GameObjectHandle.h"

using namespace mye::core;
using namespace mye::lua;

using namespace luapp11;

namespace mye
{

	namespace lua
	{

		void __script_reload(const mye::core::String name)
		{

			auto r = ResourceTypeManager::GetSingleton().GetResource("LuaScript", name);

			if (r)
			{
				r->Unload();
				r->Load();
			}

		}

		void BindScripts(State state)
		{

			state
			[

				Function("ReloadScript", &__script_reload),

				Class<LuaModule>(MYE_LUA_LUAMODULE).

					Function("LoadBehaviour",            &LuaModule::LoadBehaviour).
					Function("LoadProcedure",            &LuaModule::LoadProcedure).
					Function("LoadScriptResourceLoader", &LuaModule::LoadScriptResourceLoader),

				Class<Script, mye::core::Resource>(MYE_LUA_SCRIPT),

				//Class<BehaviourScript, Script>(MYE_LUA_BEHAVIOURSCRIPT),

				Class<ProcedureScript, Script>(MYE_LUA_PROCEDURESCRIPT).

					Function("Run", &ProcedureScript::Run)

			];

			/*module(L)
			[

				def("ReloadScript", &__script_reload),

				class_<LuaModule>(MYE_LUA_LUAMODULE).

					def("LoadBehaviour", &LuaModule::LoadBehaviour).
					def("LoadProcedure", &LuaModule::LoadProcedure).
					def("LoadScriptResourceLoader", &LuaModule::LoadScriptResourceLoader),

				class_<Script, mye::core::Resource>(MYE_LUA_SCRIPT),

				class_<BehaviourScript, Script>(MYE_LUA_BEHAVIOURSCRIPT),

				class_<ProcedureScript, Script>(MYE_LUA_PROCEDURESCRIPT).

					def("Run", &ProcedureScript::Run)				

			];*/

		}

	}

}