namespace mye
{

	namespace core
	{

		mye::math::Vector3 Camera::Up(void) const
		{
			return m_orientation.Rotate(mye::math::Vector3(0, 1, 0));
		}

		mye::math::Vector3 Camera::Right(void) const
		{
			return m_orientation.Rotate(mye::math::Vector3(1, 0, 0));
		}

		mye::math::Vector3 Camera::Forward(void) const
		{
			return m_orientation.Rotate(mye::math::Vector3(0, 0, 1));
		}

		void Camera::UpdateView(void)
		{
			m_viewMatrix = (const_cast<const Camera*>(this))->GetViewMatrix();
			m_viewMatrixUptodate = true;
		}

		mye::math::Matrix4 Camera::GetViewMatrix(void) const
		{

			if (m_viewMatrixUptodate)
			{
				return m_viewMatrix;
			}

			mye::math::Matrix3 rot = RotationMatrix3(m_orientation);
			mye::math::Matrix3 rotT = rot.Transpose();
			mye::math::Vector3 trans = - (rotT * m_position);

			mye::math::Matrix4 viewMatrix(rotT);
			viewMatrix.m03() = trans.x();
			viewMatrix.m13() = trans.y();
			viewMatrix.m23() = trans.z();

			return viewMatrix;

		}

		mye::math::Matrix4 Camera::GetProjectionMatrix(void) const
		{

			if (m_projectionMatrixUptodate)
			{
				return m_projectionMatrix;
			}

 			mye::math::Real yScale   = mye::math::Real(1) / mye::math::Tangent(m_fovY * mye::math::Real(0.5f));
 			mye::math::Real xScale   = yScale / m_aspectRatio;
 			mye::math::Real invDepth = mye::math::Real(1) / (m_farClipDistance - m_nearClipDistance);

			mye::math::Real Q = m_farClipDistance * invDepth;

			mye::math::Matrix4 projectionMatrix(0);

			projectionMatrix.m00() = xScale;
			projectionMatrix.m11() = yScale;
			projectionMatrix.m22() = Q;
			projectionMatrix.m32() = mye::math::Real(1);
			projectionMatrix.m23() = - Q * m_nearClipDistance;

			return projectionMatrix;

		}

		const mye::math::Matrix4& Camera::GetViewMatrix(void)
		{

			if (!m_viewMatrixUptodate)
			{
				UpdateView();
			}

			return m_viewMatrix;
		}

		const mye::math::Matrix4& Camera::GetProjectionMatrix(void)
		{

			if (!m_projectionMatrixUptodate)
			{
				UpdateProjection();
			}

			return m_projectionMatrix;

		}

		void Camera::UpdateProjection(void)
		{
			m_projectionMatrix = const_cast<const Camera*>(this)->GetProjectionMatrix();
			m_projectionMatrixUptodate = true;
		}

		mye::math::Frustum Camera::GetFrustum(void) const
		{
			return (m_frustumUptodate ? m_frustum :
				mye::math::Frustum(
					m_position,
					Forward(), Up(), Right(),
					m_nearClipDistance, m_farClipDistance,
					GetFovX(), GetFovY())
				);
		}

		const mye::math::Frustum& Camera::GetFrustum(void)
		{

			if (!m_frustumUptodate)
			{
				UpdateFrustum();
			}

			return m_frustum;

		}

		void Camera::UpdateFrustum(void)
		{
			m_frustum = const_cast<const Camera*>(this)->GetFrustum();
			m_frustumUptodate = true;
		}

	}

}