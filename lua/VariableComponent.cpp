#include "VariableComponent.h"

#include <mye/core/VariableComponent.h>
#include <Eigen/Eigen>

#include <luabind/luabind.hpp>

#include "Types.h"

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
					def(constructor<>()).
					property("value", &VariableComponent<T>::Get, &VariableComponent<T>::Set)

			];

		}

		void BindVariableComponent(lua_State *L)
		{

			BindVariableComponent<float>(L, MYE_LUA_FLOAT_COMPONENT);
			BindVariableComponent<int>(L, MYE_LUA_INT_COMPONENT);
			BindVariableComponent<bool>(L, MYE_LUA_BOOL_COMPONENT);
			BindVariableComponent<std::string>(L, MYE_LUA_STRING_COMPONENT);
			BindVariableComponent<Eigen::Vector3f>(L, MYE_LUA_VEC3_COMPONENT);
			BindVariableComponent<Eigen::Vector3i>(L, MYE_LUA_VEC3I_COMPONENT);

		}

	}

}