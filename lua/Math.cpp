#include "Math.h"
#include "Types.h"
#include "Converters.h"

#include "MathWrappers.h"

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include <sstream>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

using namespace luabind;
using namespace mye::math;

#define __MYE_LUA_MAKE_IJ_FROM_SIZE(N, S) BOOST_PP_CAT(BOOST_PP_DIV(N, S), BOOST_PP_MOD(N, S))

#define __MYE_LUA_MAKE_PREFIX_IJ(PREFIX, N, S) BOOST_PP_CAT(PREFIX, __MYE_LUA_MAKE_IJ_FROM_SIZE(N, S))

#define __MYE_LUA_DEFINE_MATRIX_PROPERTIES(N, I, S) \
	property \
	BOOST_PP_LPAREN() \
		BOOST_PP_STRINGIZE(BOOST_PP_CAT(m, __MYE_LUA_MAKE_IJ_FROM_SIZE(I, S))) BOOST_PP_COMMA() \
		__MYE_LUA_MAKE_PREFIX_IJ(&MatrixType::Get, I, S) BOOST_PP_COMMA() \
		__MYE_LUA_MAKE_PREFIX_IJ(&MatrixType::Set, I, S) \
	BOOST_PP_RPAREN() \
	BOOST_PP_IIF(0, BOOST_PP_EMPTY(), .)

#define __MYE_LUA_DEFINE_MATRIX2_PROPERTIES() BOOST_PP_REPEAT(4,  __MYE_LUA_DEFINE_MATRIX_PROPERTIES, 2)
#define __MYE_LUA_DEFINE_MATRIX3_PROPERTIES() BOOST_PP_REPEAT(9,  __MYE_LUA_DEFINE_MATRIX_PROPERTIES, 3)
#define __MYE_LUA_DEFINE_MATRIX4_PROPERTIES() BOOST_PP_REPEAT(16, __MYE_LUA_DEFINE_MATRIX_PROPERTIES, 4)

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
				property("w", &__vec_get<4, T, 3>, &__vec_set<4, T, 3>).

				def("Clamp", (VectorType (VectorType::*) (T, T)) &VectorType::Clamp).
				def("Clamp", (VectorType (VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

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
				property("z", &__vec_get<3, T, 2>, &__vec_set<3, T, 2>).

				def("Clamp", (VectorType (VectorType::*) (T, T)) &VectorType::Clamp).
				def("Clamp", (VectorType (VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

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
					property("y", &__vec_get<2, T, 1>, &__vec_set<2, T, 1>).

					def("Clamp", (VectorType (VectorType::*) (T, T)) &VectorType::Clamp).
					def("Clamp", (VectorType (VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

				];

		}

		void BindQuaternion(lua_State *L, const char *classname)
		{

			module(L)
			[

				class_<Quaternionf>(classname).

					def(constructor<>()).
					def(constructor<float, float, float, float>()).
					def(constructor<const Matrix<float, 3, 1>&, float>()).

					def("__tostring", &__quat_tostring<float>).

					property("w", &__quat_get<float, 3>, &__quat_set<float, 3>).
					property("x", &__quat_get<float, 0>, &__quat_set<float, 0>).
					property("y", &__quat_get<float, 1>, &__quat_set<float, 1>).
					property("z", &__quat_get<float, 2>, &__quat_set<float, 2>)

			];

		}

		template <typename T>
		void BindMatrix3(lua_State *L, const char *classname)
		{

			typedef Matrix<T, 3, 3> MatrixType;

			module(L)
				[

					class_<MatrixType>(classname).

					def(constructor<>()).
					def(constructor<T>()).
					def(constructor<const MatrixType&>()).

					__MYE_LUA_DEFINE_MATRIX3_PROPERTIES()

					def("Inverse", &MatrixType::Inverse).
					def("Transpose", &MatrixType::Transpose).
					def("Determinant", &Matrix4f::Determinant).

					def(const_self * const_self).
					def(const_self + const_self).

					def(const_self * other<T>()).
					def(other<T>() * const_self).

					def(const_self == const_self)

				];

		}

		template <typename T>
		void BindMatrix4(lua_State *L, const char *classname)
		{

			typedef Matrix<T, 4, 4> MatrixType;

			module(L)
			[

				class_<MatrixType>(classname).

					def(constructor<>()).
					def(constructor<T>()).
					def(constructor<const MatrixType&>()).
					def(constructor<const Matrix<T, 3, 3> &>()).

					__MYE_LUA_DEFINE_MATRIX4_PROPERTIES()

					def("Inverse", &MatrixType::Inverse).
					def("Transpose", &MatrixType::Transpose).
					def("Determinant", &Matrix4f::Determinant).

					def(const_self * const_self).
					def(const_self + const_self).

					def(const_self * other<T>()).
					def(other<T>() * const_self).

					def(const_self == const_self)

			];

		}

		void BindTransform(lua_State *L, const char *classname)
		{

			module(L)
			[

				class_<Transform>(classname).

					def(constructor<>()).
					def(constructor<const Matrix<Real, 3, 1> &, const Quaternion &, const Matrix<Real, 3, 1> &>()).

					property("orientation", &Transform::GetOrientation, &Transform::SetOrientation).
					property("position", &Transform::GetPosition, &Transform::SetPosition).
					property("scale", &Transform::GetScale, &Transform::SetScale).

					property("matrix", &Transform::GetSRTMatrix)

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

			BindMatrix3<Real>(L, MYE_LUA_MAT3);
			BindMatrix4<Real>(L, MYE_LUA_MAT4);

			BindQuaternion(L, MYE_LUA_QUATERNION);

			BindTransform(L, MYE_LUA_TRANSFORM);

			module(L, "Math")
			[

				def("TranslationMatrix4", &TranslationMatrix4<Real>),
				def("RotationMatrix3", &RotationMatrix3<Real>),
				def("RotationMatrix4", &RotationMatrix4<Real>),
				def("RotationTranslationMatrix4", &RotationTranslationMatrix4<Real>),
				def("ScaleMatrix4", &ScaleMatrix4<Real>)

			];

		}

	}

}