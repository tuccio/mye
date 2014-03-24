#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		inline Matrix<T, 3, 3> RotationMatrix3(const QuaternionTempl<T> &q);

		template <typename T>
		inline Matrix<T, 4, 4> RotationMatrix4(const QuaternionTempl<T> &q);

		template <typename T>
		Matrix<T, 4, 4> RotationTranslationMatrix4(const QuaternionTempl<T> &q, const Matrix<T, 3, 1> &t);

	}

}

#include "QuaternionOperations.inl"