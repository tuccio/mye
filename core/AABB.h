#pragma once

#include <mye/math/Math.h>
#include <vector>

namespace mye
{

	namespace core
	{

		class AABB
		{
		public:


			enum Corners
			{
				NEAR_LEFT_BOTTOM = 0,
				NEAR_RIGHT_BOTTOM = 1,
				NEAR_RIGHT_TOP = 2,
				NEAR_LEFT_TOP = 3,
				FAR_LEFT_TOP = 4,
				FAR_RIGHT_TOP = 5,
				FAR_RIGHT_BOTTOM = 6,
				FAR_LEFT_BOTTOM = 7
			};

			AABB(void);
			AABB::AABB(const mye::math::Vector3f &min,
				const mye::math::Vector3f &max);
			~AABB(void);

			mye::math::Vector3f GetCenter(void) const;
			mye::math::Vector3f GetHalfExtents(void) const;

			mye::math::Vector3f GetMinimum(void) const;
			mye::math::Vector3f GetMaximum(void) const;

			std::vector<mye::math::Vector3f> GetCorners(void) const;

			AABB TransformAffine(const mye::math::Matrix4f &t);

		private:

			mye::math::Vector3f m_min;
			mye::math::Vector3f m_max;

		};

	}

}
