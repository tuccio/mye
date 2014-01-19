#pragma once

#include <Eigen/Eigen>
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
			AABB::AABB(const Eigen::Vector3f &min,
				const Eigen::Vector3f &max);
			~AABB(void);

			Eigen::Vector3f GetCenter(void) const;
			Eigen::Vector3f GetHalfExtents(void) const;

			Eigen::Vector3f GetMinimum(void) const;
			Eigen::Vector3f GetMaximum(void) const;

			std::vector<Eigen::Vector3f> GetCorners(void) const;

			AABB TransformAffine(const Eigen::Matrix4f &t);

		private:

			Eigen::Vector3f m_min;
			Eigen::Vector3f m_max;

		};

	}

}
