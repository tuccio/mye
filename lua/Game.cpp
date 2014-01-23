#pragma once

#include "Types.h"

#include <mye/core/Game.h>
#include <mye/core/InputModule.h>

#include <luabind/luabind.hpp>

#include "Game.h"
#include "DirectX11.h"
#include "Script.h"
#include "GameObjectHandle.h"
#include "VariableComponent.h"
#include "WindowsFunctions.h"
#include "InputModule.h"

#include "LuaModule.h"
#include "LuaScriptCaller.h"

#include <mye/d3d11/DX11Module.h>

using namespace luabind;
using namespace mye::core;
using namespace mye::dx11;

namespace mye
{

	namespace lua
	{

		IWindow* __game_get_window(Game &game);

		void __graphics_module_reinterpret(const std::string &type);

		void BindGame(lua_State *L)
		{

			BindGameObjectHandle(L);

			module(L)
			[

				class_<Game>(MYE_LUA_GAME).

					def("GetMainWindow", &__game_get_window).
					def("Quit", &Game::Quit),

				class_<GameObjectsModule>(MYE_LUA_GAMEOBJECTSMODULE).

					def("Create", (GameObjectHandle (GameObjectsModule::*) (void)) &GameObjectsModule::Create).
					def("Create", (GameObjectHandle (GameObjectsModule::*) (const std::string&)) &GameObjectsModule::Create).

					def("Find", &GameObjectsModule::Find),

				class_<GraphicsModule>(MYE_LUA_GRAPHICSMODULE).

					def("GetWindow", (IWindow* (GraphicsModule::*) (void)) &GraphicsModule::GetWindow).
					def("HasWindow", &GraphicsModule::HasWindow).
					def("Reinterpret", &__graphics_module_reinterpret)

			];

			BindInputModule(L);
			BindKeyboard(L);

			BindScripts(L);
			BindVariableComponent(L);

			BindWindow(L);
			BindWindowsFunctions(L);

			BindDirectX11(L);

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

		IWindow* __game_get_window(Game &game)
		{
			return game.GetGraphicsModule()->GetWindow();
		}

		void __graphics_module_reinterpret(const std::string &type)
		{

			Game& game = Game::GetSingleton();
			lua_State *L = static_cast<LuaModule*>(game.GetScriptModule())->GetLuaState();

			if (type == "DX11")
			{
				DX11Module &dx11 = *static_cast<DX11Module*>(game.GetGraphicsModule());
				globals(L)["Graphics"] = object(L, boost::ref(dx11));
			}

		}

	}

}