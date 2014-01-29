#pragma once

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		class Camera
		{

		public:

			Camera(void);
			~Camera(void);

			void LookAt(const mye::math::Vector3f &position,
				const mye::math::Vector3f &up,
				const mye::math::Vector3f &target);

			mye::math::Quaternionf GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternionf &orientation);

			mye::math::Vector3f GetPosition(void) const;
			void SetPosition(const mye::math::Vector3f &position);

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

			inline void UpdateView(void);
			inline void UpdateProjection(void);

			inline mye::math::Matrix4f GetViewMatrix(void) const;
			inline mye::math::Matrix4f GetProjectionMatrix(void) const;

			inline const mye::math::Matrix4f& GetViewMatrix(void);
			inline const mye::math::Matrix4f& GetProjectionMatrix(void);

		private:

			mye::math::Quaternionf m_orientation;
			mye::math::Vector3f m_position;

			float m_fovY;
			float m_aspectRatio;
			float m_nearClipDistance;
			float m_farClipDistance;

			mye::math::Matrix4f m_viewMatrix;
			mye::math::Matrix4f m_projectionMatrix;

			bool m_viewMatrixUptodate;
			bool m_projectionMatrixUptodate;

		};

	}

}

#include "Camera.inl"
