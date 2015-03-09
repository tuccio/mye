#include "VariableComponent.h"

#include "Converters.h"
#include "Math.h"
#include "Types.h"

#include <mye/core/GameObject.h>
#include <mye/core/TransformComponent.h>
#include <mye/core/VariableComponent.h>

#include <mye/math/Math.h>

#include <type_traits>

using namespace mye::core;
using namespace luapp11;

namespace mye
{

	namespace lua
	{


		/*template <typename T>
		boost::reference_wrapper<T> __mye_variable_component_ref(VariableComponent<T> &vc)
		{
			return boost::ref(*vc.GetPointer());
		}*/

		template <typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, void>::type BindVariableComponent(State state, const char *name)
		{

			/*module(L)
			[

				class_<VariableComponent<T>, Component>(name).
				def(constructor<const mye::core::String&>()).
				property("value", &VariableComponent<T>::Get, &VariableComponent<T>::Set)

			];*/

		}

		template <typename T>
		typename std::enable_if<std::is_arithmetic<T>::value == false, void>::type BindVariableComponent(State state, const char *name)
		{

			/*module(L)
			[

				class_<VariableComponent<T>, Component>(name).
				def(constructor<const mye::core::String&>()).
				property("value", &__mye_variable_component_ref<T>)

			];*/

		}


		/*template <typename T>
		void BindVariableComponent(lua_State *L, const char *name)
		{

			module(L)
			[

				class_<VariableComponent<T>, Component>(name).
				def(constructor<const mye::core::String&>()).
				property("value", &__mye_variable_component_ref)

			];

		}*/

		void BindVariableComponent(State state)
		{

			BindVariableComponent<int>                         (state, MYE_LUA_VAR_INT);
			BindVariableComponent<bool>                        (state, MYE_LUA_VAR_BOOL);
			BindVariableComponent<mye::math::Real>             (state, MYE_LUA_VAR_REAL);
			BindVariableComponent<mye::core::String>           (state, MYE_LUA_VAR_STRING);
			BindVariableComponent<mye::math::Vector3>          (state, MYE_LUA_VAR_VEC3);
			BindVariableComponent<mye::math::Vector3i>         (state, MYE_LUA_VAR_VEC3I);
			BindVariableComponent<mye::math::Quaternion>       (state, MYE_LUA_VAR_QUATERNION);
			BindVariableComponent<mye::math::Transform>        (state, MYE_LUA_VAR_TRANSFORM);
			BindVariableComponent<mye::core::GameObjectHandle> (state, MYE_LUA_VAR_GAMEOBJECTHANDLE);

		}

	}

}