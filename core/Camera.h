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

			void LookAt(const mye::math::Vector3 &position,
				const mye::math::Vector3 &up,
				const mye::math::Vector3 &target);

			mye::math::Quaternion GetOrientation(void) const;
			void SetOrientation(const mye::math::Quaternion &orientation);

			mye::math::Vector3 GetPosition(void) const;
			void SetPosition(const mye::math::Vector3 &position);

			void SetProjection(float fovy, float aspect, float zNear, float zFar);

			float GetNearClipDistance(void) const;
			void SetNearClipDistance(float near);

			float GetFarClipDistance(void) const;
			void SetFarClipDistance(float far);

			void SetClipDistances(float near, float far);

			float GetClipAspectRatio(void) const;
			void SetClipAspectRatio(float aspect);

			float GetFovY(void) const;
			void SetFovY(float fovy);

			float GetFovX(void) const;

			float GetFovYRadians(void) const;
			float GetFovXRadians(void) const;

			void Roll(float angle);
			void Pitch(float angle);
			void Yaw(float angle);

			inline mye::math::Vector3 Up(void) const;
			inline mye::math::Vector3 Right(void) const;
			inline mye::math::Vector3 Forward(void) const;

			inline void UpdateView(void);
			inline void UpdateProjection(void);
			inline void UpdateFrustum(void);

			inline mye::math::Matrix4 GetViewMatrix(void) const;
			inline mye::math::Matrix4 GetProjectionMatrix(void) const;

			inline const mye::math::Matrix4& GetViewMatrix(void);
			inline const mye::math::Matrix4& GetProjectionMatrix(void);

			inline mye::math::Frustum GetFrustum(void) const;
			inline const mye::math::Frustum& GetFrustum(void);

		private:

			mye::math::Quaternion m_orientation;
			mye::math::Vector3 m_position;

			float m_fovY;
			float m_aspectRatio;
			float m_nearClipDistance;
			float m_farClipDistance;

			mye::math::Matrix4 m_viewMatrix;
			mye::math::Matrix4 m_projectionMatrix;

			bool m_viewMatrixUptodate;
			bool m_projectionMatrixUptodate;
			bool m_frustumUptodate;

			mye::math::Frustum m_frustum;

		};

	}

}

#include "Camera.inl"
