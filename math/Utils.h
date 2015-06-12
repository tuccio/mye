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
			struct IsVector : std::false_type { };

			template <typename T>
			struct IsVector<Matrix<T, 2, 1>> : std::true_type { };

			template <typename T>
			struct IsVector<Matrix<T, 3, 1>> : std::true_type { };

			template <typename T>
			struct IsVector<Matrix<T, 4, 1>> : std::true_type { };

			template <typename T>
			struct IsFPOrFPVector :
				std::integral_constant<bool,
				std::is_floating_point<T>::value ||
				IsVector<float>::value ||
				IsVector<double>::value> { };

			template <typename T>
			struct IsIntOrIntVector :
				std::integral_constant<bool,
				std::is_integral<T>::value ||
				IsVector<int>::value ||
				IsVector<unsigned int>::value> { };

			template <typename T>
			using SFINAEStrippedFP = std::enable_if_t<IsFPOrFPVector<T>::value, StrippedType<T>>;

			template <typename T>
			using SFINAEStrippedScalar = std::enable_if_t<IsFPOrFPVector<T>::value || IsIntOrIntVector<T>::value, StrippedType<T>>;

			template <typename T>
			using SFINAEVector = std::enable_if_t<IsVector<T>::value, T>;

			template <typename T>
			using SFINAEScalar = std::enable_if_t<std::is_arithmetic<T>::value, T>;

		}

	}

}