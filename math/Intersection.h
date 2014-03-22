#pragma once

namespace mye
{

	namespace math
	{

		template <typename T>
		inline bool Intersect(const Rayt<T> &Rayt,
			const Sphere<T> &sphere);

		template <typename T>
		inline bool Intersect(const Rayt<T> &Rayt,
			const Sphere<T> &sphere,
			Matrix<T, 3, 1> &intersectionPoint);

		template <typename T>
		inline VolumeSide Intersect(const AABBt<T> &a,
			const AABBt<T> &b);

		template <typename T>
		inline VolumeSide Intersect(const AABBt<T> &AABBt,
			const Frustumt<T> &frustum);

		template <typename T>
		inline VolumeSide Intersect(const Frustumt<T> &frustum,
			const AABBt<T> &AABBt);

		template <typename T>
		inline VolumeSide Intersect(const Frustumt<T> &a,
			const Frustumt<T> &b);

	}

}

#include "Intersection.inl"