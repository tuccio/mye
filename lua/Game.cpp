#pragma once

#include "Types.h"

#include <mye/core/Game.h>
#include <luabind/luabind.hpp>

#include "LuaEnvironment.h"
#include "GameObjectHandle.h"
#include "VariableComponent.h"

using namespace luabind;
using namespace mye::core;

namespace mye
{

	namespace lua
	{

		void BindGame(lua_State *L)
		{

			BindGameObjectHandle(L);

			module(L)
			[
				class_<Game>(MYE_LUA_GAME)
			];

			module(L)
			[

				class_<GameObjectsModule>(MYE_LUA_GAMEOBJECTSMODULE).

					def("Create", (GameObjectHandle (GameObjectsModule::*) (void)) &GameObjectsModule::Create).
					def("Create", (GameObjectHandle (GameObjectsModule::*) (const std::string&)) &GameObjectsModule::Create).

					def("Find", &GameObjectsModule::Find)

			];

			BindVariableComponent(L);

		}

	}

}