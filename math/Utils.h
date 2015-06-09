#pragma once

#include <type_traits>

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename T>
			using StrippedType = typename std::remove_const<std::remove_reference_t<T>>::type;

			template <typename T>
			struct IsFPOrFPVector :
				std::integral_constant<bool,
				std::is_floating_point<T>::value ||
				std::is_same<Matrix<float, 2, 1>, T>::value ||
				std::is_same<Matrix<float, 3, 1>, T>::value ||
				std::is_same<Matrix<float, 4, 1>, T>::value ||
				std::is_same<Matrix<double, 2, 1>, T>::value ||
				std::is_same<Matrix<double, 3, 1>, T>::value ||
				std::is_same<Matrix<double, 4, 1>, T>::value> { };

			template <typename T>
			struct IsIntOrIntVector :
				std::integral_constant<bool,
				std::is_integral<T>::value ||
				std::is_same<Matrix<int, 2, 1>, T>::value ||
				std::is_same<Matrix<int, 3, 1>, T>::value ||
				std::is_same<Matrix<int, 4, 1>, T>::value ||
				std::is_same<Matrix<unsigned int, 2, 1>, T>::value ||
				std::is_same<Matrix<unsigned int, 3, 1>, T>::value ||
				std::is_same<Matrix<unsigned int, 4, 1>, T>::value> { };

			template <typename T>
			using SFINAEStrippedFP = std::enable_if_t<IsFPOrFPVector<T>::value, StrippedType<T>>;

			template <typename T>
			using SFINAEStrippedScalar = std::enable_if_t<IsFPOrFPVector<T>::value || IsIntOrIntVector<T>::value, StrippedType<T>>;

		}

	}

}