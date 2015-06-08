namespace mye
{

	namespace math
	{

		template <typename T>
		inline AABBTempl<T> BoundingAABB(const AABBTempl<T> & aabb);

		template <typename T>
		inline AABBTempl<T> BoundingAABB(const Triangle<T> & triangle);

		template <typename T>
		inline AABBTempl<T> BoundingAABB(const FrustumTempl<T> & frustum);


	}

}

#include "BoundingVolumes.inl"