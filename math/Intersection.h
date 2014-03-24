#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		inline bool Intersect(const RayTempl<T> &ray,
			const Sphere<T> &sphere);

		template <typename T>
		inline bool Intersect(const RayTempl<T> &ray,
			const Sphere<T> &sphere,
			Matrix<T, 3, 1> &intersectionPoint);

		template <typename T>
		inline VolumeSide Intersect(const AABBTempl<T> &a,
			const AABBTempl<T> &b);

		template <typename T>
		inline VolumeSide Intersect(const AABBTempl<T> &aabb,
			const FrustumTempl<T> &frustum);

		template <typename T>
		inline VolumeSide Intersect(const FrustumTempl<T> &frustum,
			const AABBTempl<T> &aabb);

		template <typename T>
		inline VolumeSide Intersect(const FrustumTempl<T> &a,
			const FrustumTempl<T> &b);

	}

}

#include "Intersection.inl"