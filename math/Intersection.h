#pragma once

#include "IntersectionCalculator.h"

#include "IntersectionAABBvsAABB.h"
#include "IntersectionRayvsAABB.h"
#include "IntersectionRayvsSphere.h"
#include "IntersectionAABBvsFrustum.h"
#include "IntersectionAABBvsSphere.h"

#include <type_traits>

namespace mye
{

	namespace math
	{

		/*
			Intersection between geometry types (volumes, rays).
			The function is commutative, optional arguments may differ depending on the pair of volume types to intersect.
		*/

		template <typename V1, typename V2, typename ... OptionalArguments>
		__MYE_MATH_INLINE std::enable_if_t<detail::IntersectionCalculator<V1, V2>::implemented::value, bool>
			Intersect(const V1 & v1, const V2 & v2, OptionalArguments && ... args)
		{
			return detail::IntersectionCalculator<V1, V2>::Intersect(v1, v2, std::forward<OptionalArguments>(args) ...);
		}

		template <typename V1, typename V2, typename ... OptionalArguments>
		__MYE_MATH_INLINE std::enable_if_t<!std::is_same<V1, V2>::value && detail::IntersectionCalculator<V2, V1>::implemented::value, bool>
			Intersect(const V1 & v1, const V2 & v2, OptionalArguments && ... args)
		{
			return detail::IntersectionCalculator<V2, V1>::Intersect(v2, v1, std::forward<OptionalArguments>(args) ...);
		}

	}

}