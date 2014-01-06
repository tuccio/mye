#pragma once

#include "Types.h"

#include <mye/core/Game.h>

#include <luabind/luabind.hpp>

#include "Game.h"
#include "Script.h"
#include "GameObjectHandle.h"
#include "VariableComponent.h"
#include "WindowsFunctions.h"

#include "LuaModule.h"
#include "LuaScriptCaller.h"

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
				class_<Game>(MYE_LUA_GAME).

					def("GetMainWindow", &Game::GetMainWindow)

			];

			module(L)
			[

				class_<GameObjectsModule>(MYE_LUA_GAMEOBJECTSMODULE).

					def("Create", (GameObjectHandle (GameObjectsModule::*) (void)) &GameObjectsModule::Create).
					def("Create", (GameObjectHandle (GameObjectsModule::*) (const std::string&)) &GameObjectsModule::Create).

					def("Find", &GameObjectsModule::Find)

			];

			BindScripts(L);
			BindVariableComponent(L);

			BindWindow(L);
			BindWindowsFunctions(L);

		}

		void BindWindow(lua_State *L)
		{

			module(L)
			[

				class_<IWindow>(MYE_LUA_WINDOW).

					def("Create", (bool (IWindow::*) (void)) &IWindow::Create).
					def("Destroy", &IWindow::Destroy).

					def("Show", &IWindow::Show).
					def("Hide", &IWindow::Hide).

					def("GetCaption", &IWindow::GetCaption).
					def("SetCaption", &IWindow::SetCaption).

					def("GetSize", &IWindow::GetSize).
					def("SetSize", &IWindow::SetSize).

					def("GetPosition", &IWindow::GetPosition).
					def("SetPosition", &IWindow::SetPosition)

			];

		}

	}

}