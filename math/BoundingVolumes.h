namespace mye
{

	namespace math
	{


		template <typename T>
		inline AABB<T> BoundingAABB(const Triangle<T> &triangle);

		template <typename T>
		inline AABB<T> BoundingAABB(const Frustum<T> &frustum);


	}

}

#include "BoundingVolumes.inl"