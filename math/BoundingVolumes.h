namespace mye
{

	namespace math
	{

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const AABBTempl<T> & aabb);

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const Triangle<T> & triangle);

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const FrustumTempl<T> & frustum);

		template <typename T>
		__MYE_MATH_INLINE AABBTempl<T> BoundingAABB(const SphereTempl<T> & sphere);


	}

}

#include "BoundingVolumes.inl"