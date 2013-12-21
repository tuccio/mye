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

		void RegisterVector3(lua_State *L)
		{

			module(L)
			[

				class_<Vector3f>(MYE_LUA_VEC3).

					def(constructor<>()).
					def(constructor<float, float, float>()).
					def(constructor<const Vector3f&>()).

					def("__add", (Vector3f (*) (const Vector3f&, const Vector3f&)) &__vec_add<3, float>).
					def("__add", (Vector3f (*) (const Vector3f&, float)) &__vec_add<3, float>).
					def("__sub", (Vector3f (*) (const Vector3f&, const Vector3f&)) &__vec_sub<3, float>).
					def("__sub", (Vector3f (*) (const Vector3f&, float)) &__vec_sub<3, float>).
					def("__mul", (Vector3f (*) (const Vector3f&, const Vector3f&)) &__vec_mul<3, float>).
					def("__mul", (Vector3f (*) (const Vector3f&, float)) &__vec_mul<3, float>).
					def("__div", (Vector3f (*) (const Vector3f&, const Vector3f&)) &__vec_div<3, float>).
					def("__div", (Vector3f (*) (const Vector3f&, float)) &__vec_div<3, float>).

					def(const_self == const_self).

					def("__tostring", &__vec_tostring<3, float>).

					property("x", &__vec3_getx<float>, &__vec3_setx<float>).
					property("y", &__vec3_gety<float>, &__vec3_sety<float>).
					property("z", &__vec3_getz<float>, &__vec3_setz<float>)

			];

		}

	}

}