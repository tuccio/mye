#pragma once

#include "Types.h"

#include <mye/core/Components.h>
#include <mye/core/Game.h>
#include <mye/core/InputModule.h>

#include "Game.h"
#include "DirectX11.h"
#include "Converters.h"
#include "GameObjectHandle.h"
#include "InputModule.h"
#include "Scripting.h"
#include "VariableComponent.h"
#include "WindowsFunctions.h"

#include "LuaModule.h"

#include <mye/d3d11/DX11Module.h>
#include <mye/math/Math.h>

#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/empty.hpp>

using namespace mye::core;
using namespace mye::dx11;

using namespace luapp11;


namespace mye
{

	namespace lua
	{

		IWindow* __game_get_window(Game &game);

		void __graphics_module_reinterpret(const mye::core::String &type);

		void BindGame(luapp11::State state)
		{

			BindGameObjectHandle(state);			

			state
			[

				Class<PointLight>(MYE_LUA_POINTLIGHT).

					Constructor<const mye::math::Vector3&, mye::math::Real, const mye::math::Vector3&, mye::math::Real>().

					Property("position",  &PointLight::position).
					Property("color",     &PointLight::color).
					Property("intensity", &PointLight::intensity).
					Property("range",     &PointLight::range),

				Enum<LightType>("LightType")

					("Directional", LightType::DIRECTIONAL)
					("Pointlight",  LightType::POINTLIGHT)
					("Spotlight",   LightType::SPOTLIGHT),

				Class<Light>(MYE_LUA_LIGHT).

					Constructor<>().
					Constructor<const PointLight &>().

					Property("position",  &Light::GetPosition,  &Light::SetPosition).
					Property("color",     &Light::GetColor,     &Light::SetColor).
					Property("intensity", &Light::GetIntensity, &Light::SetIntensity).
					Property("direction", &Light::GetDirection, &Light::SetDirection).
					Property("spotAngle", &Light::GetSpotAngle, &Light::SetSpotAngle).
					Property("range",     &Light::GetRange,     &Light::SetRange).
					Property("type",      &Light::GetType,      &Light::SetType),

				Class<Camera>(MYE_LUA_CAMERA).

					Function("LookAt", &Camera::LookAt).

					Function("Roll",  &Camera::Roll).
					Function("Yaw",   &Camera::Yaw).
					Function("Pitch", &Camera::Pitch).

					Function("RayCast", &Camera::RayCast).

					Function("GetViewMatrix",       (const mye::math::Matrix4& (Camera::*) ()) &Camera::GetViewMatrix).
					Function("GetProjectionMatrix", (const mye::math::Matrix4& (Camera::*) ()) &Camera::GetProjectionMatrix).

					Property("position",    &Camera::GetPosition,         &Camera::SetPosition).
					Property("orientation", &Camera::GetOrientation,      &Camera::SetOrientation).
					Property("fovx",        &Camera::GetFovX).
					Property("fovy",        &Camera::GetFovY,             &Camera::SetFovY).
					Property("near",        &Camera::GetNearClipDistance, &Camera::SetNearClipDistance).
					Property("far",         &Camera::GetFarClipDistance,  &Camera::SetFarClipDistance).

					Property("right",   &Camera::Right).
					Property("forward", &Camera::Forward).
					Property("up",      &Camera::Up),

				/*Class<Material>(MYE_LUA_MATERIAL).
					
					Constructor<>().

					Property("color",     &Material::color).
					Property("metallic",  &Material::metallic).
					Property("specular",  &Material::specular).
					Property("roughness", &Material::roughness),*/

				Class<Game>(MYE_LUA_GAME).

					Function("GetMainWindow", &__game_get_window).
					Function("Quit", &Game::Quit),

				Class<GameObjectsModule, GameObjectsManager>(MYE_LUA_GAMEOBJECTSMODULE).

					Function("Create", (GameObjectHandle(GameObjectsModule::*) (void)) &GameObjectsModule::Create).
					Function("Create", (GameObjectHandle(GameObjectsModule::*) (const mye::core::String&)) &GameObjectsModule::Create).

					Function("CreateEntity", (GameObjectHandle(GameObjectsModule::*) (const mye::core::String&)) &GameObjectsModule::CreateEntity).
					Function("CreateEntity", (GameObjectHandle(GameObjectsModule::*) (const mye::core::String&, const mye::core::String&)) &GameObjectsModule::CreateEntity).

					Function("Find", &GameObjectsModule::Find),

				Class<GraphicsModule>(MYE_LUA_GRAPHICSMODULE).

					Function("HasWindow",   &GraphicsModule::HasWindow).
					Function("Reinterpret", &__graphics_module_reinterpret).

					Property("window", (IWindow* (GraphicsModule::*) (void)) &GraphicsModule::GetWindow).

					Property("clearColor", &GraphicsModule::GetClearColor, &GraphicsModule::SetClearColor).
					Property("vsync",      &GraphicsModule::GetVSync, &GraphicsModule::SetVSync).
					Property("fps",        &GraphicsModule::GetFPS),

				Class<Component>(MYE_LUA_COMPONENT),

				Class<TransformComponent, Component>(MYE_LUA_TRANSFORM_COMPONENT).

					Property("position",    &TransformComponent::Position,    &TransformComponent::SetPosition).
					Property("orientation", &TransformComponent::Orientation, &TransformComponent::SetOrientation).
					Property("scale",       &TransformComponent::Scale,       &TransformComponent::SetScale).

					Property("up",      &TransformComponent::Up).
					Property("right",   &TransformComponent::Right).
					Property("forward", &TransformComponent::Forward),

				Class<RigidBodyComponent, Component>(MYE_LUA_RIGIDBODY_COMPONENT).

					Constructor<>().
					Constructor<BulletCollisionShapePointer, mye::math::Real>().
					Constructor<BulletCollisionShapePointer, mye::math::Real, const mye::math::Vector3&, const mye::math::Quaternion&>().

					Function("ApplyForce", &RigidBodyComponent::ApplyForce).
					Function("ApplyImpulse", &RigidBodyComponent::ApplyImpulse).

					Property("velocity", &RigidBodyComponent::GetVelocity,       &RigidBodyComponent::SetVelocity).
					Property("position", &RigidBodyComponent::GetPosition,       &RigidBodyComponent::SetPosition).
					Property("mass",     &RigidBodyComponent::GetMass,           &RigidBodyComponent::SetMass).
					Property("shape",    &RigidBodyComponent::GetCollisionShape, &RigidBodyComponent::SetCollisionShape),

				Class<Text2DComponent, Component>(MYE_LUA_TEXT2D_COMPONENT).

					Constructor<>().
					Constructor<const mye::math::Vector2i &, FontPointer, const mye::core::String &>().

					Property("text",      &Text2DComponent::GetText,      &Text2DComponent::SetText).
					Property("position",  &Text2DComponent::GetPosition,  &Text2DComponent::SetPosition).
					Property("font",      &Text2DComponent::GetFont,      &Text2DComponent::SetFont).
					Property("pointsize", &Text2DComponent::GetPointSize, &Text2DComponent::SetPointSize).
					Property("color",     &Text2DComponent::GetColor,     &Text2DComponent::SetColor),

				Class<RenderComponent, Component>(MYE_LUA_RENDER_COMPONENT).

					Constructor<>().

					Property("mesh",     &RenderComponent::GetMesh,            &RenderComponent::SetMesh).
					Property("matrix",   &RenderComponent::GetModelMatrix,     &RenderComponent::SetModelMatrix).
					Property("material", &RenderComponent::GetMaterial,        &RenderComponent::SetMaterial),

				Class<LightComponent, Component, Light>(MYE_LUA_LIGHT_COMPONENT).

					Constructor<>().
					Constructor<const Light&>(),

				Class<CameraComponent, Component, Camera>(MYE_LUA_CAMERA_COMPONENT).

					Constructor<>()

			];

			BindInputModule(state);

			BindScripts(state);
			BindVariableComponent(state);

			BindWindow(state);
			BindWindowsFunctions(state);

			BindDirectX11(state);

		}

		void BindWindow(luapp11::State state)
		{

			state
			[

				Class<IWindow>(MYE_LUA_WINDOW).

					Function("Create", (bool (IWindow::*) (void)) &IWindow::Create).
					Function("Destroy", &IWindow::Destroy).

					Function("Focus", &IWindow::Focus).
					Function("Show",  &IWindow::Show).
					Function("Hide",  &IWindow::Hide).

					Property("caption",  &IWindow::GetCaption, &IWindow::SetCaption).
					Property("size",     &IWindow::GetSize, &IWindow::SetSize).
					Property("position", &IWindow::GetPosition, &IWindow::SetPosition)

			];

		}

		IWindow* __game_get_window(Game &game)
		{
			return game.GetGraphicsModule()->GetWindow();
		}

		void __graphics_module_reinterpret(const mye::core::String &type)
		{

			/*Game& game = Game::GetSingleton();
			lua_State *L = static_cast<LuaModule*>(game.GetScriptModule())->GetLuaState();

			if (type == "DX11")
			{
				DX11Module &dx11 = *static_cast<DX11Module*>(game.GetGraphicsModule());
				globals(L)["Graphics"] = object(L, boost::ref(dx11));
			}*/

		}

	}

}