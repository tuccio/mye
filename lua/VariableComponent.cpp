#include "VariableComponent.h"
#include "Math.h"
#include "Types.h"
#include "Converters.h"
#include "Alignment.h"

#include <mye/core/VariableComponent.h>
#include <mye/core/TransformComponent.h>
#include <mye/math/Math.h>

#include <luabind/luabind.hpp>

using namespace mye::core;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		template <typename T>
		void BindVariableComponent(lua_State *L, const char *name)
		{

			module(L)
			[

				class_<VariableComponent<T>, Component>(name).
					def(constructor<const mye::core::String&>()).
					property("value", &VariableComponent<T>::Get, &VariableComponent<T>::Set)

			];

		}

		void BindTransformComponent(lua_State *L, const char *name)
		{

			module(L)
			[

				class_<TransformComponent, Component>(name).

					property("position", &TransformComponent::GetPosition, &TransformComponent::SetPosition).
					property("orientation", &TransformComponent::GetOrientation, &TransformComponent::SetOrientation).
					property("scale", &TransformComponent::GetScale, &TransformComponent::SetScale).

					property("up", &TransformComponent::Up).
					property("right", &TransformComponent::Right).
					property("forward", &TransformComponent::Forward)


			];

		}

		void BindVariableComponent(lua_State *L)
		{

			BindVariableComponent<mye::math::Real>(L, MYE_LUA_FLOAT_COMPONENT);
			BindVariableComponent<int>(L, MYE_LUA_INT_COMPONENT);
			BindVariableComponent<bool>(L, MYE_LUA_BOOL_COMPONENT);
			BindVariableComponent<mye::core::String>(L, MYE_LUA_STRING_COMPONENT);
			BindVariableComponent<mye::math::Vector3>(L, MYE_LUA_VEC3_COMPONENT);
			BindVariableComponent<mye::math::Vector3i>(L, MYE_LUA_VEC3I_COMPONENT);
			BindVariableComponent<mye::math::Quaternion>(L, MYE_LUA_QUATERNION_COMPONENT);
			BindVariableComponent<mye::math::Transform>(L, MYE_LUA_VTRANSFORM_COMPONENT);

			BindTransformComponent(L, MYE_LUA_TRANSFORM_COMPONENT);

		}

	}

}