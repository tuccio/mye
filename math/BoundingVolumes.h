namespace mye
{

	namespace math
	{

		template <typename T>
		inline AABBt<T> BoundingAABB(const AABBt<T> &AABBt);

		template <typename T>
		inline AABBt<T> BoundingAABB(const Triangle<T> &triangle);

		template <typename T>
		inline AABBt<T> BoundingAABB(const Frustumt<T> &frustum);


	}

}

#include "BoundingVolumes.inl"