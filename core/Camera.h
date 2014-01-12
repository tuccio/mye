#pragma once

#include <Eigen/Eigen>

namespace mye
{

	namespace core
	{

		class Camera
		{

		public:

			Camera(void);
			~Camera(void);

			Eigen::Quaternionf GetOrientation(void) const;
			void SetOrientation(const Eigen::Quaternionf &orientation);

			Eigen::Vector3f GetPosition(void) const;
			void SetPosition(const Eigen::Vector3f &position);

			float GetNearClipDistance(void) const;
			void SetNearClipDistance(float nearClipDistance);

			float GetFarClipDistance(void) const;
			void SetFarClipDistance(float farClipDistance);

		private:

			Eigen::Quaternionf m_orientation;
			Eigen::Vector3f m_position;
			float m_nearClipDistance;
			float m_farClipDistance;

		};

	}

}

