#pragma once

#include "Types.h"

#include <mye/core/Components.h>
#include <mye/core/Game.h>
#include <mye/core/InputModule.h>

#include <luabind/luabind.hpp>

#include "Game.h"
#include "DirectX11.h"
#include "Scripting.h"
#include "GameObjectHandle.h"
#include "VariableComponent.h"
#include "WindowsFunctions.h"
#include "InputModule.h"
#include "Converters.h"

#include "LuaModule.h"

#include <mye/d3d11/DX11Module.h>

using namespace luabind;
using namespace mye::core;
using namespace mye::dx11;

namespace mye
{

	namespace lua
	{

		IWindow* __game_get_window(Game &game);

		void __graphics_module_reinterpret(const mye::core::String &type);

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
					def("Create", (GameObjectHandle (GameObjectsModule::*) (const mye::core::String&)) &GameObjectsModule::Create).

					def("CreateEntity", (GameObjectHandle (GameObjectsModule::*) (const mye::core::String&)) &GameObjectsModule::CreateEntity).
					def("CreateEntity", (GameObjectHandle (GameObjectsModule::*) (const mye::core::String&, const mye::core::String&)) &GameObjectsModule::CreateEntity).

					def("Find", &GameObjectsModule::Find),

				class_<GraphicsModule>(MYE_LUA_GRAPHICSMODULE).

					def("GetWindow", (IWindow* (GraphicsModule::*) (void)) &GraphicsModule::GetWindow).
					def("HasWindow", &GraphicsModule::HasWindow).
					def("Reinterpret", &__graphics_module_reinterpret),

				class_<Component>(MYE_LUA_COMPONENT),

				class_<RigidBodyComponent, Component>(MYE_LUA_RIGIDBODY_COMPONENT).
					def(constructor<BulletCollisionShapePointer, mye::math::Real>()).
					property("velocity", &RigidBodyComponent::GetVelocity, &RigidBodyComponent::SetVelocity).
					property("position", &RigidBodyComponent::GetPosition, &RigidBodyComponent::SetPosition),


				class_<Text2DComponent, Component>(MYE_LUA_TEXT2D_COMPONENT).
					def(constructor<>()).
					property("text", &Text2DComponent::GetText, &Text2DComponent::SetText).
					property("position", &Text2DComponent::GetPosition, &Text2DComponent::SetPosition).
					property("font", &Text2DComponent::GetFont, &Text2DComponent::SetFont).
					property("pointsize", &Text2DComponent::GetPointSize, &Text2DComponent::SetPointSize).
					property("color", &Text2DComponent::GetColor, &Text2DComponent::SetColor)

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

		void __graphics_module_reinterpret(const mye::core::String &type)
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