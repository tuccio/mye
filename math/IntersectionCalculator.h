#pragma once

#include <type_traits>

namespace mye
{

	namespace math
	{

		namespace detail
		{

			template <typename V1, typename V2>
			struct IntersectionCalculator { };

			template <typename V1, typename V2>
			struct IntersectionCalculatorBase
			{
				typedef std::integral_constant<bool, true> implemented;
			};

		}

	}

}