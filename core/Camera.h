#pragma once

#include <Eigen/Eigen>

#include "AABB.h"

namespace mye
{

	namespace core
	{

		class Camera
		{

		public:

			Camera(void);
			~Camera(void);

			void LookAt(const Eigen::Vector3f &position,
				const Eigen::Vector3f &up,
				const Eigen::Vector3f &target);

			Eigen::Quaternionf GetOrientation(void) const;
			void SetOrientation(const Eigen::Quaternionf &orientation);

			Eigen::Vector3f GetPosition(void) const;
			void SetPosition(const Eigen::Vector3f &position);

			void SetFrustum(float fovy, float aspect, float zNear, float zFar);

			float GetNearClipDistance(void) const;
			void SetNearClipDistance(float near);

			float GetFarClipDistance(void) const;
			void SetFarClipDistance(float far);

			void SetClipDistances(float near, float far);

			float GetClipAspectRatio(void) const;
			void SetClipAspectRatio(float aspect);

			float GetFovY(void) const;
			float GetFovX(void) const;

			void Roll(float angle);
			void Pitch(float angle);
			void Yaw(float angle);

			void UpdateView(void);
			void UpdateProjection(void);

			inline const Eigen::Matrix4f& GetViewMatrix(void) const;
			inline const Eigen::Matrix4f& GetProjectionMatrix(void) const;
			inline const Eigen::Matrix4f GetViewProjectionMatrix(void) const;

		private:

			Eigen::Quaternionf m_orientation;
			Eigen::Vector3f m_position;

			float m_fovY;
			float m_aspectRatio;
			float m_nearClipDistance;
			float m_farClipDistance;

			Eigen::Matrix4f m_viewMatrix;
			Eigen::Matrix4f m_projectionMatrix;

		};

		const Eigen::Matrix4f& Camera::GetViewMatrix(void) const
		{
			return m_viewMatrix;
		}

		const Eigen::Matrix4f& Camera::GetProjectionMatrix(void) const
		{
			return m_projectionMatrix;
		}

		const Eigen::Matrix4f Camera::GetViewProjectionMatrix(void) const
		{
			return m_viewMatrix * m_projectionMatrix;
			//return m_projectionMatrix * m_viewMatrix;
		}

	}

}

