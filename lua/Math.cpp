#include "Converters.h"
#include "Math.h"
#include "MetaMethodsOverload.h"
#include "Types.h"

#include "MathWrappers.h"

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

#include <algorithm>
#include <sstream>

#include <boost/variant.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/mod.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

using namespace luapp11;
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

		// TODO: Use different overloads for the vector swizzle templated on the vector size

		static const std::vector<char> __math_vec_elements[] =
		{
			{ 'x', 'u', 'r' },
			{ 'y', 'v', 'g' },
			{ 'z', 'b' },
			{ 'w', 'a' }
		};

		template <typename T, int SourceVectorSize>
		static void __math_vec_push(lua_State * L, const Matrix<T, SourceVectorSize, 1> &sv, const std::vector<int> &swizzleResult)
		{

			Object rvalue;

			switch (swizzleResult.size())
			{

			case 2:
			{

				Matrix<T, 2, 1> ov(sv[swizzleResult[0]], sv[swizzleResult[1]]);
				rvalue = Object(L, ov);

			}
				break;

			case 3:
			{

				Matrix<T, 3, 1> ov(sv[swizzleResult[0]], sv[swizzleResult[1]], sv[swizzleResult[2]]);
				rvalue = Object(L, ov);
			}

				break;

			case 4:
			{

				Matrix<T, 4, 1> ov(sv[swizzleResult[0]], sv[swizzleResult[1]], sv[swizzleResult[2]], sv[swizzleResult[3]]);
				rvalue = Object(L, ov);
			}

				break;

			default:

				break;

			}

			if (rvalue)
			{
				rvalue.Push();
			}
			else
			{
				lua_pushnil(L);
			}

		}

		static int __math_vec_swizzle(const char *swizzle, int vectorSize, std::vector<int> &swizzleResult)
		{

			int size = 0;
			swizzleResult.clear();

			while (*swizzle)
			{

				bool found = false;

				for (int i = 0; i < vectorSize; i++)
				{

					auto it = std::find(__math_vec_elements[i].begin(), __math_vec_elements[i].end(), *swizzle);

					if (it != __math_vec_elements[i].end())
					{
						found = true;
						swizzleResult.push_back(i);
					}

				}

				if (!found)
				{
					return -1;
				}

				swizzle++;

			}

			return size;

		}

		typedef boost::variant<
			mye::math::Vector2,
			mye::math::Vector2i,
			mye::math::Vector3,
			mye::math::Vector3i,
			mye::math::Vector4,
			mye::math::Vector4i> VectorVariant;

		VectorVariant __math_vec_from_object(Object object)
		{

			VectorVariant v;
			//if (object.IsCastable<mye::math::Vector3>)
			/*if (auto v3 = object_cast_nothrow<mye::math::Vector3>(stackObject))
			{
				v = v3.get();
			}
			else if (auto v4 = object_cast_nothrow<mye::math::Vector4>(stackObject))
			{
				v = v4.get();
			}
			else if (auto v2 = object_cast_nothrow<mye::math::Vector2>(stackObject))
			{
				v = v2.get();
			}
			else if (auto v4i = object_cast_nothrow<mye::math::Vector4i>(stackObject))
			{
				v = v4i.get();
			}
			else if (auto v3i = object_cast_nothrow<mye::math::Vector3i>(stackObject))
			{
				v = v3i.get();
			}
			else if (auto v2i = object_cast_nothrow<mye::math::Vector2i>(stackObject))
			{
				v = v2i.get();
			}*/

			return v;

		}

		struct VectorVariantVisitor :
			boost::static_visitor<bool>
		{

			template <typename T, int N>
			bool operator() (const mye::math::Matrix<T, N, 1> & v)
			{

				std::vector<int> swizzleResult;

				__math_vec_swizzle(swizzle, N, swizzleResult);

				__math_vec_push(L, v, swizzleResult);
				return true;

			}

			lua_State *L;
			const char *swizzle;

		};

		int VectorSwizzle(lua_State *L)
		{

			int returnValues = 0;
			Object stackObject(FromStack(L, 1));

			VectorVariant v = __math_vec_from_object(stackObject);

			if (!v.empty())
			{

				VectorVariantVisitor visitor;
				visitor.L = L;
				visitor.swizzle = lua_tostring(L, 2);

				if (v.apply_visitor(visitor))
				{
					returnValues = 1;
				}

			}

			return returnValues;

		}

		template <typename T>
		void BindVector4(State state, const char * classname)
		{

			typedef Matrix<T, 4, 1> VectorType;
			
			state
			[

				Class<VectorType>(classname).

					Constructor<>().
					Constructor<T>().
					Constructor<T, T, T, T>().
					Constructor<const Matrix<T, 2, 1> &, T, T>().
					Constructor<const Matrix<T, 3, 1> &, T>().
					Constructor<const VectorType &>().

					Operator(Operand<const VectorType &>() + Operand<const VectorType &>()).
					Operator(Operand<const VectorType &>() + Operand<T>()).
					Operator(Operand<T>() + Operand<const VectorType &>()).

					Operator(Operand<const VectorType &>() - Operand<const VectorType &>()).
					Operator(Operand<const VectorType &>() - Operand<T>()).
					Operator(Operand<T>() - Operand<const VectorType &>()).

					Operator(Operand<const VectorType &>() * Operand<const VectorType &>()).
					Operator(Operand<const VectorType &>() * Operand<T>()).
					Operator(Operand<T>() * Operand<const VectorType &>()).

					Operator(Operand<const VectorType &>() / Operand<const VectorType &>()).
					Operator(Operand<const VectorType &>() / Operand<T>()).
					Operator(Operand<T>() / Operand<const VectorType &>()).

					Operator(Operand<const VectorType &>() == Operand<const VectorType &>()).

					Operator(- Operand<const VectorType &>()).

					ToString(&__vec_tostring<4, T>).

					Property("x", &__vec_get<4, T, 0>, &__vec_set<4, T, 0>).
					Property("y", &__vec_get<4, T, 1>, &__vec_set<4, T, 1>).
					Property("z", &__vec_get<4, T, 2>, &__vec_set<4, T, 2>).
					Property("w", &__vec_get<4, T, 3>, &__vec_set<4, T, 3>).

					Function("Normalize", &VectorType::Normalize)/*.

					Function("Clamp", (VectorType(VectorType::*) (T, T)) &VectorType::Clamp).
					Function("Clamp", (VectorType(VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)*/

			];

			/*module(L)
			[

				class_<VectorType>(classname).

				def(constructor<>()).
				def(constructor<T>()).
				def(constructor<T, T, T, T>()).
				def(constructor<const Matrix<T, 2, 1> &, T, T>()).
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

				def("Normalize", &VectorType::Normalize).

				def("Clamp", (VectorType (VectorType::*) (T, T)) &VectorType::Clamp).
				def("Clamp", (VectorType (VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

			];*/

			//RegisterIndexOverload<VectorType>(&VectorSwizzle);

		}

		template <typename T>
		void BindVector3(State state, const char *classname)
		{

			typedef Matrix<T, 3, 1> VectorType;

			state
			[

				Class<VectorType>(classname).

				Constructor<>().
				Constructor<T>().
				Constructor<T, T, T>().
				Constructor<const Matrix<T, 2, 1> &, T>().
				Constructor<const VectorType &>().

				Operator(Operand<const VectorType &>() + Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() + Operand<T>()).
				Operator(Operand<T>() + Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() - Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() - Operand<T>()).
				Operator(Operand<T>() - Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() * Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() * Operand<T>()).
				Operator(Operand<T>() * Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() / Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() / Operand<T>()).
				Operator(Operand<T>() / Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() == Operand<const VectorType &>()).

				Operator(- Operand<const VectorType &>()).

				ToString(&__vec_tostring<3, T>).

				Property("x", &__vec_get<3, T, 0>, &__vec_set<3, T, 0>).
				Property("y", &__vec_get<3, T, 1>, &__vec_set<3, T, 1>).
				Property("z", &__vec_get<3, T, 2>, &__vec_set<3, T, 2>).

				Function("Normalize", &VectorType::Normalize).

				Function("Clamp", (VectorType(VectorType::*) (T, T)) &VectorType::Clamp).
				Function("Clamp", (VectorType(VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

			];

			//module(L)
			//[

			//	class_<VectorType>(classname).

			//	def(constructor<>()).
			//	def(constructor<T>()).
			//	def(constructor<T, T, T>()).
			//	def(constructor<const Matrix<T, 2, 1>&, T>()).
			//	def(constructor<const VectorType&>()).

			//	def("__add", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_add<3, T>).
			//	def("__add", (VectorType (*) (const VectorType&, T)) &__vec_add<3, T>).
			//	def("__add", (VectorType (*) (T, const VectorType&)) &__vec_add<3, T>).

			//	def("__sub", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_sub<3, T>).
			//	def("__sub", (VectorType (*) (const VectorType&, T)) &__vec_sub<3, T>).
			//	def("__sub", (VectorType (*) (T, const VectorType&)) &__vec_sub<3, T>).

			//	def("__mul", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_mul<3, T>).
			//	def("__mul", (VectorType (*) (const VectorType&, T)) &__vec_mul<3, T>).
			//	def("__mul", (VectorType (*) (T, const VectorType&)) &__vec_mul<3, T>).

			//	def("__div", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_div<3, T>).
			//	def("__div", (VectorType (*) (const VectorType&, T)) &__vec_div<3, T>).
			//	def("__div", (VectorType (*) (T, const VectorType&)) &__vec_div<3, T>).

			//	def(const_self == const_self).

			//	def(- const_self).

			//	def("__tostring", &__vec_tostring<3, T>).

			//	property("x", &__vec_get<3, T, 0>, &__vec_set<3, T, 0>).
			//	property("y", &__vec_get<3, T, 1>, &__vec_set<3, T, 1>).
			//	property("z", &__vec_get<3, T, 2>, &__vec_set<3, T, 2>).

			//	/*property("x", (T (VectorType::*) ()) &VectorType::x).
			//	property("y", (T (VectorType::*) ()) &VectorType::y).
			//	property("z", (T (VectorType::*) ()) &VectorType::z).*/

			//	def("Normalize", &VectorType::Normalize).

			//	def("Clamp", (VectorType (VectorType::*) (T, T)) &VectorType::Clamp).
			//	def("Clamp", (VectorType (VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

			//];

			//RegisterIndexOverload<VectorType>(&VectorSwizzle);

		}

		template <typename T>
		void BindVector2(State state, const char *classname)
		{

			typedef Matrix<T, 2, 1> VectorType;

			state
			[

				Class<VectorType>(classname).

				Constructor<>().
				Constructor<T>().
				Constructor<T, T>().
				Constructor<const VectorType &>().

				Operator(Operand<const VectorType &>() + Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() + Operand<T>()).
				Operator(Operand<T>() + Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() - Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() - Operand<T>()).
				Operator(Operand<T>() - Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() * Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() * Operand<T>()).
				Operator(Operand<T>() * Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() / Operand<const VectorType &>()).
				Operator(Operand<const VectorType &>() / Operand<T>()).
				Operator(Operand<T>() / Operand<const VectorType &>()).

				Operator(Operand<const VectorType &>() == Operand<const VectorType &>()).

				Operator(- Operand<const VectorType &>()).

				ToString(&__vec_tostring<2, T>).

				Property("x", &__vec_get<2, T, 0>, &__vec_set<2, T, 0>).
				Property("y", &__vec_get<2, T, 1>, &__vec_set<2, T, 1>).

				Function("Normalize", &VectorType::Normalize).

				Function("Clamp", (VectorType(VectorType::*) (T, T)) &VectorType::Clamp).
				Function("Clamp", (VectorType(VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

			];

		/*	module(L)
				[

					class_<VectorType>(classname).

					def(constructor<>()).
					def(constructor<T>()).
					def(constructor<T, T>()).
					def(constructor<const VectorType&>()).

					def("__add", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_add<2, T>).
					def("__add", (VectorType (*) (const VectorType&, T)) &__vec_add<2, T>).
					def("__add", (VectorType (*) (T, const VectorType&)) &__vec_add<2, T>).

					def("__sub", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_sub<2, T>).
					def("__sub", (VectorType (*) (const VectorType&, T)) &__vec_sub<2, T>).
					def("__sub", (VectorType (*) (T, const VectorType&)) &__vec_sub<2, T>).

					def("__mul", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_mul<2, T>).
					def("__mul", (VectorType (*) (const VectorType&, T)) &__vec_mul<2, T>).
					def("__mul", (VectorType (*) (T, const VectorType&)) &__vec_mul<2, T>).

					def("__div", (VectorType (*) (const VectorType&, const VectorType&)) &__vec_div<2, T>).
					def("__div", (VectorType (*) (const VectorType&, T)) &__vec_div<2, T>).
					def("__div", (VectorType (*) (T, const VectorType&)) &__vec_div<2, T>).

					def(const_self == const_self).

					def(- const_self).

					def("__tostring", &__vec_tostring<2, T>).

					property("x", &__vec_get<2, T, 0>, &__vec_set<2, T, 0>).
					property("y", &__vec_get<2, T, 1>, &__vec_set<2, T, 1>).

					def("Normalize", &VectorType::Normalize).

					def("Clamp", (VectorType (VectorType::*) (T, T)) &VectorType::Clamp).
					def("Clamp", (VectorType (VectorType::*) (const VectorType&, const VectorType&)) &VectorType::Clamp)

				];

			RegisterIndexOverload<VectorType>(&VectorSwizzle);
*/
		}

		void BindQuaternion(State state, const char *classname)
		{

			state
			[

				Class<Quaternion>(classname).

					Constructor<>().
					Constructor<Real, Real, Real, Real>().
					Constructor<const Vector3&, Real>().
					Constructor<const Quaternion &>().

					Operator(Operand<const Quaternion &>() * Operand<const Quaternion &>()).

					//Operator(Operand<const Quaternion &>() == Operand<const Quaternion &>()).

					Function("Inverse",   &Quaternion::Inverse).
					Function("Conjugate", &Quaternion::Conjugate).

					Function("Normalize", &Quaternion::Normalize).
					Function("Rotate",    &Quaternion::Rotate).

					ToString(&__quat_tostring<Real>).

					Property("w", &__quat_get<Real, 3>, &__quat_set<Real, 3>).
					Property("x", &__quat_get<Real, 0>, &__quat_set<Real, 0>).
					Property("y", &__quat_get<Real, 1>, &__quat_set<Real, 1>).
					Property("z", &__quat_get<Real, 2>, &__quat_set<Real, 2>)

			];

			/*module(L)
			[

				class_<Quaternion>(classname).

					def(constructor<>()).
					def(constructor<Real, Real, Real, Real>()).
					def(constructor<const Matrix<Real, 3, 1>&, Real>()).

					def(const_self * const_self).

					def("Inverse", &Quaternion::Inverse).
					def("Conjugate", &Quaternion::Conjugate).

					def("Normalize", &Quaternion::Normalize).

					def("__tostring", &__quat_tostring<float>).

					property("w", &__quat_get<Real, 3>, &__quat_set<Real, 3>).
					property("x", &__quat_get<Real, 0>, &__quat_set<Real, 0>).
					property("y", &__quat_get<Real, 1>, &__quat_set<Real, 1>).
					property("z", &__quat_get<Real, 2>, &__quat_set<Real, 2>)

			];*/

		}

		template <typename T>
		void BindMatrix3(State state, const char *classname)
		{

			typedef Matrix<T, 3, 3> MatrixType;

			state
			[

				Class<MatrixType>(classname).

					Constructor<>().
					Constructor<T>().
					Constructor<const MatrixType &>().

					//__MYE_LUA_DEFINE_MATRIX3_PROPERTIES()

					Function("Inverse",     &MatrixType::Inverse).
					Function("Transpose",   &MatrixType::Transpose).
					Function("Determinant", &MatrixType::Determinant).

					Operator(Operand<const MatrixType &>() == Operand<const MatrixType &>()).

					Operator(Operand<const MatrixType &>() * Operand<const MatrixType &>()).
					Operator(Operand<const MatrixType &>() + Operand<const MatrixType &>()).
					//Operator(Operand<const MatrixType &>() - Operand<const MatrixType &>()).

					Operator(Operand<const MatrixType &>() * Operand<T>()).
					//Operator(Operand<const MatrixType &>() / Operand<T>()).

					Operator(Operand<const MatrixType &>() * Operand<const Vector3 &>())


			];

		/*	module(L)
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

				def(const_self * other<Matrix<T, 3, 1>>()).

				def(const_self == const_self)

			];*/

		}

		template <typename T>
		void BindMatrix4(State state, const char *classname)
		{

			typedef Matrix<T, 4, 4> MatrixType;

			state
			[

				Class<MatrixType>(classname).

					Constructor<>().
					Constructor<T>().
					Constructor<const MatrixType &>().
					Constructor<const Matrix<T, 3, 3> &>().

					//__MYE_LUA_DEFINE_MATRIX4_PROPERTIES()

					Function("Inverse",     &MatrixType::Inverse).
					Function("Transpose",   &MatrixType::Transpose).
					Function("Determinant", &MatrixType::Determinant).

					Operator(Operand<const MatrixType &>() == Operand<const MatrixType &>()).

					Operator(Operand<const MatrixType &>() * Operand<const MatrixType &>()).
					Operator(Operand<const MatrixType &>() + Operand<const MatrixType &>()).
					//Operator(Operand<const MatrixType &>() - Operand<const MatrixType &>()).

					Operator(Operand<const MatrixType &>() * Operand<T>()).
					//Operator(Operand<const MatrixType &>() / Operand<T>()).

					Operator(Operand<const MatrixType &>() * Operand<const Vector4 &>())


			];

			/*module(L)
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

					def(const_self * other<Matrix<T, 4, 1>>()).

					def(const_self == const_self)

			];*/

		}

		void BindTransform(State state, const char *classname)
		{

			state
			[

				Class<Transform>(classname).

					Constructor<>().
					Constructor<const Matrix<Real, 3, 1> &, const Quaternion &, const Matrix<Real, 3, 1> &>().

					Property("orientation", &Transform::GetOrientation, &Transform::SetOrientation).
					Property("position",    &Transform::GetPosition,    &Transform::SetPosition).
					Property("scale",       &Transform::GetScale,       &Transform::SetScale).

					Property("matrix",      &Transform::GetSRTMatrix)

			];

			/*module(L)
			[

				class_<Transform>(classname).

					def(constructor<>()).
					def(constructor<const Matrix<Real, 3, 1> &, const Quaternion &, const Matrix<Real, 3, 1> &>()).

					property("orientation", &Transform::GetOrientation, &Transform::SetOrientation).
					property("position", &Transform::GetPosition, &Transform::SetPosition).
					property("scale", &Transform::GetScale, &Transform::SetScale).

					property("matrix", &Transform::GetSRTMatrix)

			];*/

		}

		void BindVolumes(State state, const char *classname)
		{

			state
			[

				Class<AABB>(classname).

					Function("FromCenterHalfExtents", &AABB::FromCenterHalfExtents).
					Function("FromMinMax",            &AABB::FromMinMax).

					Property("center",      &AABB::GetCenter).
					Property("halfextents", &AABB::GetHalfExtents).

					Property("min", &AABB::GetMinimum).
					Property("max", &AABB::GetMaximum)

			];

			/*module(L)
			[

				class_<Volume<Real>>(classname).

					def("TransformAffine", &Volume<Real>::TransformAffine).
					def("Intersect", &Volume<Real>::Intersect),

				class_<AABB, Volume<Real>>(classname).

					def("FromCenterHalfExtents", &AABB::FromCenterHalfExtents).
					def("FromMinMax", &AABB::FromMinMax).

					property("center", &AABB::GetCenter).
					property("halfextents", &AABB::GetHalfExtents).

					property("min", &AABB::GetMinimum).
					property("max", &AABB::GetMaximum)

			];*/

		}

		void BindRay(State state, const char *classname)
		{

			state
			[

				Class<Ray>(classname).

				Constructor<>().
				Constructor<const mye::math::Vector3 &, const mye::math::Vector3 &>().

				Function("Evaluate",  &Ray::Evaluate).

				Property("origin",    &Ray::GetOrigin,    &Ray::SetOrigin).
				Property("direction", &Ray::GetDirection, &Ray::SetDirection)

			];

			/*module(L)
			[

				class_<Ray>(classname).

					def(constructor<>()).
					def(constructor<const mye::math::Vector3&, const mye::math::Vector3&>()).

					def("Evaluate", &Ray::Evaluate).

					property("origin",    &Ray::GetOrigin,    &Ray::SetOrigin).
					property("direction", &Ray::GetDirection, &Ray::SetDirection)

			];*/

		}

		void BindMath(State state)
		{

			BindVector2<Real> (state, MYE_LUA_VEC2);
			BindVector2<int>  (state, MYE_LUA_VEC2I);
							  
			BindVector3<Real> (state, MYE_LUA_VEC3);
			BindVector3<int>  (state, MYE_LUA_VEC3I);
							  
			BindVector4<Real> (state, MYE_LUA_VEC4);
			BindVector4<int>  (state, MYE_LUA_VEC4I);
							  
			BindMatrix3<Real> (state, MYE_LUA_MAT3);
			BindMatrix4<Real> (state, MYE_LUA_MAT4);
							  
			BindQuaternion    (state, MYE_LUA_QUATERNION);
							  
			BindTransform     (state, MYE_LUA_TRANSFORM);
							  
			BindVolumes       (state, MYE_LUA_AABB);

			BindRay           (state, MYE_LUA_RAY);


			state
			[

				Namespace("Math")
				[
				
					Function("TranslationMatrix4",         &TranslationMatrix4<Real>),
					Function("RotationMatrix3",            &RotationMatrix3<Real>),
					Function("RotationMatrix4",            &RotationMatrix4<Real>),
					Function("RotationTranslationMatrix4", &RotationTranslationMatrix4<Real>),
					Function("ScaleMatrix4",               &ScaleMatrix4<Real>),

					Function("EulerAngles", &EulerAngles<Real>),

					Function("Degrees", (Real(*) (Real)) &Degrees<Real>),
					Function("Degrees", (Vector3(*) (const Vector3&)) &Degrees<Real>),

					Function("Radians", (Real(*) (Real)) &Radians<Real>),
					Function("Radians", (Vector3(*) (const Vector3&)) &Radians<Real>)

				]

			];

			/*module(L, "Math")
			[

				def("TranslationMatrix4",         &TranslationMatrix4<Real>),
				def("RotationMatrix3",            &RotationMatrix3<Real>),
				def("RotationMatrix4",            &RotationMatrix4<Real>),
				def("RotationTranslationMatrix4", &RotationTranslationMatrix4<Real>),
				def("ScaleMatrix4",               &ScaleMatrix4<Real>),

				def("EulerAngles",                &EulerAngles<Real>),

				def("Degrees", (Real (*) (Real)) &Degrees<Real>),
				def("Degrees", (Vector3 (*) (const Vector3&)) &Degrees<Real>),

				def("Radians", (Real(*) (Real)) &Radians<Real>),
				def("Radians", (Vector3(*) (const Vector3&)) &Radians<Real>)

			];*/

		}

	}

}