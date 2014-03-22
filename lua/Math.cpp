#include "Math.h"
#include "Types.h"
#include "Converters.h"

#include "MathWrappers.h"

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include <sstream>

using namespace luabind;
using namespace mye::math;

namespace mye
{

	namespace lua
	{

		template <typename T>
		void BindVector4(lua_State *L, const char *classname)
		{

			typedef Matrix<T, 4, 1> VectorType;

			module(L)
				[

					class_<VectorType>(classname).

					def(constructor<>()).
					def(constructor<T>()).
					def(constructor<T, T, T, T>()).
					def(constructor<const Matrix<T, 3, 1>&, T>()).
					def(constructor<const VectorType&>()).

					def("__add", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_add<4, T>).
					def("__add", (VectorType (*) (const VectorType&, T)) &__vec_add<4, T>).

					def("__sub", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_sub<4, T>).
					def("__sub", (VectorType (*) (const VectorType&, T)) &__vec_sub<4, T>).

					def("__mul", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_mul<4, T>).
					def("__mul", (VectorType (*) (const VectorType&, T)) &__vec_mul<4, T>).

					def("__div", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_div<4, T>).
					def("__div", (VectorType (*) (const VectorType&, T)) &__vec_div<4, T>).

					def(const_self == const_self).

					def(- const_self).

					def("__tostring", &__vec_tostring<4, T>).

					property("x", &__vec_get<4, T, 0>, &__vec_set<4, T, 0>).
					property("y", &__vec_get<4, T, 1>, &__vec_set<4, T, 1>).
					property("z", &__vec_get<4, T, 2>, &__vec_set<4, T, 2>).
					property("w", &__vec_get<4, T, 3>, &__vec_set<4, T, 3>)

				];

		}

		template <typename T>
		void BindVector3(lua_State *L, const char *classname)
		{

			typedef Matrix<T, 3, 1> VectorType;

			module(L)
			[

				class_<VectorType>(classname).

				def(constructor<>()).
				def(constructor<T>()).
				def(constructor<T, T, T>()).
				def(constructor<const Matrix<T, 2, 1>&, T>()).
				def(constructor<const VectorType&>()).

				def("__add", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_add<3, T>).
				def("__add", (VectorType (*) (const VectorType&, T)) &__vec_add<3, T>).

				def("__sub", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_sub<3, T>).
				def("__sub", (VectorType (*) (const VectorType&, T)) &__vec_sub<3, T>).

				def("__mul", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_mul<3, T>).
				def("__mul", (VectorType (*) (const VectorType&, T)) &__vec_mul<3, T>).

				def("__div", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_div<3, T>).
				def("__div", (VectorType (*) (const VectorType&, T)) &__vec_div<3, T>).

				def(const_self == const_self).

				def(- const_self).

				def("__tostring", &__vec_tostring<3, T>).

				property("x", &__vec_get<3, T, 0>, &__vec_set<3, T, 0>).
				property("y", &__vec_get<3, T, 1>, &__vec_set<3, T, 1>).
				property("z", &__vec_get<3, T, 2>, &__vec_set<3, T, 2>)

			];

		}

		template <typename T>
		void BindVector2(lua_State *L, const char *classname)
		{

			typedef Matrix<T, 2, 1> VectorType;

			module(L)
				[

					class_<VectorType>(classname).

					def(constructor<>()).
					def(constructor<T>()).
					def(constructor<T, T>()).
					def(constructor<const VectorType&>()).

					def("__add", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_add<2, T>).
					def("__add", (VectorType (*) (const VectorType&, T)) &__vec_add<2, T>).

					def("__sub", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_sub<2, T>).
					def("__sub", (VectorType (*) (const VectorType&, T)) &__vec_sub<2, T>).

					def("__mul", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_mul<2, T>).
					def("__mul", (VectorType (*) (const VectorType&, T)) &__vec_mul<2, T>).

					def("__div", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_div<2, T>).
					def("__div", (VectorType (*) (const VectorType&, T)) &__vec_div<2, T>).

					def(const_self == const_self).

					def(- const_self).

					def("__tostring", &__vec_tostring<2, T>).

					property("x", &__vec_get<2, T, 0>, &__vec_set<2, T, 0>).
					property("y", &__vec_get<2, T, 1>, &__vec_set<2, T, 1>)

				];

		}

		void BindQuaternion(lua_State *L, const char *classname)
		{

			module(L)
			[

				class_<Quaternionf>(classname).

					def(constructor<>()).
					def(constructor<float, float, float, float>()).

					def("__tostring", &__quat_tostring<float>).

					property("w", &__quat_get<float, 3>, &__quat_set<float, 3>).
					property("x", &__quat_get<float, 0>, &__quat_set<float, 0>).
					property("y", &__quat_get<float, 1>, &__quat_set<float, 1>).
					property("z", &__quat_get<float, 2>, &__quat_set<float, 2>)

			];

		}

		void BindTransform(lua_State *L, const char *classname)
		{

			module(L)
			[

				class_<Transform>(classname).

					def(constructor<>()).

					property("orientation", &Transform::GetOrientation, &Transform::SetOrientation).
					property("position", &Transform::GetPosition, &Transform::SetPosition).
					property("scale", &Transform::GetScale, &Transform::SetScale)

			];

		}		

		void BindMath(lua_State *L)
		{

			BindVector2<Real>(L, MYE_LUA_VEC2);
			BindVector2<int>(L, MYE_LUA_VEC2I);

			BindVector3<Real>(L, MYE_LUA_VEC3);
			BindVector3<int>(L, MYE_LUA_VEC3I);

			BindVector4<Real>(L, MYE_LUA_VEC4);
			BindVector4<int>(L, MYE_LUA_VEC4I);

			BindQuaternion(L, MYE_LUA_QUATERNION);

			BindTransform(L, MYE_LUA_TRANSFORM);

		}

	}

}