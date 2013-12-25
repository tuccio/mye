#include "Vector2.h"
#include "Types.h"

#include "__vec.h"

#include <Eigen/Eigen>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include <sstream>

using namespace luabind;
using namespace Eigen;

namespace mye
{

	namespace lua
	{

		template <typename T>
		void RegisterVector2(lua_State *L, const char *classname)
		{

			typedef Eigen::Matrix<T, 2, 1, 0, 2, 1> VectorType;

			module(L)
				[

				class_<VectorType>(classname).

				def(constructor<>()).
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

				def("__tostring", &__vec_tostring<3, T>).

				property("x", &__vec_get<2, T, 0>, &__vec_set<2, T, 0>).
				property("y", &__vec_get<2, T, 1>, &__vec_set<2, T, 1>)

			];

		}

		void RegisterVector2(lua_State *L)
		{

			RegisterVector2<float>(L, MYE_LUA_VEC2);
			RegisterVector2<int>(L, MYE_LUA_VEC2I);

		}

	}

}