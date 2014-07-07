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
#include "Alignment.h"

#include "LuaModule.h"

#include <mye/d3d11/DX11Module.h>
#include <mye/math/Math.h>

#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/empty.hpp>

#include <luabind/detail/typetraits.hpp>

using namespace luabind;
using namespace mye::core;
using namespace mye::dx11;


namespace mye
{

	namespace lua
	{

		void __mye_halt(void)
		{
			bool halt = true;
			assert(halt);
		}

		IWindow* __game_get_window(Game &game);

		void __graphics_module_reinterpret(const mye::core::String &type);

		void BindGame(lua_State *L)
		{

			BindGameObjectHandle(L);			

			module(L)
			[

				def("Halt", &__mye_halt),

				class_<PointLight>(MYE_LUA_POINTLIGHT).

				def(constructor<const mye::math::Vector3&, const mye::math::Vector3&, const mye::math::Vector3&, mye::math::Real>()).

					def_readwrite("position", &PointLight::position).
					def_readwrite("color", &PointLight::color).
					def_readwrite("intensity", &PointLight::intensity).
					def_readwrite("range", &PointLight::range),

				class_<Light>(MYE_LUA_LIGHT).

					enum_("LightType")
					[
						value("Directional", static_cast<int>(LightType::DIRECTIONAL)),
						value("Pointlight", static_cast<int>(LightType::POINTLIGHT)),
						value("Spotlight", static_cast<int>(LightType::SPOTLIGHT))
					].

					def(constructor<>()).
					def(constructor<const PointLight &>()).

					property("position", &Light::GetPosition, &Light::SetPosition).
					property("color", &Light::GetColor, &Light::SetColor).
					property("intensity", &Light::GetIntensity, &Light::SetIntensity).
					property("direction", &Light::GetDirection, &Light::SetDirection).
					property("spotAngle", &Light::GetSpotAngle, &Light::SetSpotAngle).
					property("range", &Light::GetRange, &Light::SetRange).
					property("type", &Light::GetType, &Light::SetType),

				class_<Camera>(MYE_LUA_CAMERA).

					def("Roll", &Camera::Roll).
					def("Yaw", &Camera::Yaw).
					def("Pitch", &Camera::Pitch).

					property("position", &Camera::GetPosition, &Camera::SetPosition).
					property("orientation", &Camera::GetOrientation, &Camera::SetOrientation).
					property("fovx", &Camera::GetFovX).
					property("fovy", &Camera::GetFovY, &Camera::SetFovY).

					property("right", &Camera::Right).
					property("forward", &Camera::Forward).
					property("up", &Camera::Up),

				class_<Game>(MYE_LUA_GAME).

					def("GetMainWindow", &__game_get_window).
					def("Quit", &Game::Quit),

				class_<GameObjectsModule>(MYE_LUA_GAMEOBJECTSMODULE).

					def("Create", (GameObjectHandle(GameObjectsModule::*) (void)) &GameObjectsModule::Create).
					def("Create", (GameObjectHandle(GameObjectsModule::*) (const mye::core::String&)) &GameObjectsModule::Create).

					def("CreateEntity", (GameObjectHandle(GameObjectsModule::*) (const mye::core::String&)) &GameObjectsModule::CreateEntity).
					def("CreateEntity", (GameObjectHandle(GameObjectsModule::*) (const mye::core::String&, const mye::core::String&)) &GameObjectsModule::CreateEntity).

					def("Find", &GameObjectsModule::Find),

				class_<GraphicsModule>(MYE_LUA_GRAPHICSMODULE).

					def("GetWindow", (IWindow* (GraphicsModule::*) (void)) &GraphicsModule::GetWindow).
					def("HasWindow", &GraphicsModule::HasWindow).
					def("Reinterpret", &__graphics_module_reinterpret),

				class_<Component>(MYE_LUA_COMPONENT),

				class_<TransformComponent, Component, mye::math::Transform>(MYE_LUA_TRANSFORM_COMPONENT).

					property("position", &TransformComponent::GetPosition, &TransformComponent::SetPosition).
					property("orientation", &TransformComponent::GetOrientation, &TransformComponent::SetOrientation).
					property("scale", &TransformComponent::GetScale, &TransformComponent::SetScale).

					property("up", &TransformComponent::Up).
					property("right", &TransformComponent::Right).
					property("forward", &TransformComponent::Forward),

				class_<RigidBodyComponent, Component>(MYE_LUA_RIGIDBODY_COMPONENT).

					def(constructor<>()).
					def(constructor<BulletCollisionShapePointer, mye::math::Real>()).
					def(constructor<BulletCollisionShapePointer, mye::math::Real, const mye::math::Vector3&, const mye::math::Quaternion&>()).
					property("velocity", &RigidBodyComponent::GetVelocity, &RigidBodyComponent::SetVelocity).
					property("position", &RigidBodyComponent::GetPosition, &RigidBodyComponent::SetPosition).
					property("mass", &RigidBodyComponent::GetMass, &RigidBodyComponent::SetMass).
					property("shape", &RigidBodyComponent::GetCollisionShape, &RigidBodyComponent::SetCollisionShape),

				class_<Text2DComponent, Component>(MYE_LUA_TEXT2D_COMPONENT).

					def(constructor<>()).
					def(constructor<const mye::math::Vector2i &, FontPointer, const mye::core::String &>()).
					property("text", &Text2DComponent::GetText, &Text2DComponent::SetText).
					property("position", &Text2DComponent::GetPosition, &Text2DComponent::SetPosition).
					property("font", &Text2DComponent::GetFont, &Text2DComponent::SetFont).
					property("pointsize", &Text2DComponent::GetPointSize, &Text2DComponent::SetPointSize).
					property("color", &Text2DComponent::GetColor, &Text2DComponent::SetColor),

				class_<RenderComponent, Component>(MYE_LUA_RENDER_COMPONENT).

					def(constructor<>()).
					property("matrix", &RenderComponent::GetModelMatrix, &RenderComponent::SetModelMatrix),

				class_<LightComponent, bases<Component, Light>>(MYE_LUA_LIGHT_COMPONENT).
					def(constructor<>()).
					def(constructor<const Light &>())					

			];

			BindInputModule(L);

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

					def("Focus", &IWindow::Focus).
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