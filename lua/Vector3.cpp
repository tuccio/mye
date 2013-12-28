#include "Vector3.h"
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
		void BindVector3(lua_State *L, const char *classname)
		{

			typedef Eigen::Matrix<T, 3, 1, 0, 3, 1> VectorType;

			module(L)
			[

				class_<VectorType>(classname).

				def(constructor<>()).
				def(constructor<T, T, T>()).
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

				def("__tostring", &__vec_tostring<3, T>).

				property("x", &__vec_get<3, T, 0>, &__vec_set<3, T, 0>).
				property("y", &__vec_get<3, T, 1>, &__vec_set<3, T, 1>).
				property("z", &__vec_get<3, T, 2>, &__vec_set<3, T, 2>)

			];

		}

		void BindVector3(lua_State *L)
		{

			BindVector3<float>(L, MYE_LUA_VEC3);
			BindVector3<int>(L, MYE_LUA_VEC3I);

		}

	}

}