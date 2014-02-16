#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		inline bool Intersect(const Ray<T> &ray,
			const Sphere<T> &sphere);

		template <typename T>
		inline bool Intersect(const Ray<T> &ray,
			const Sphere<T> &sphere,
			Matrix<T, 3, 1> &intersectionPoint);

		template <typename T>
		inline VolumeSide Intersect(const AABB<T> &aabb,
			const Frustum<T> &frustum);

	}

}

#include "Intersection.inl"