namespace mye
{

	namespace core
	{

		mye::math::Vector3f Camera::Up(void) const
		{
			return m_orientation.Rotate(mye::math::Vector3f(0, 1, 0));
		}

		mye::math::Vector3f Camera::Right(void) const
		{
			return m_orientation.Rotate(mye::math::Vector3f(1, 0, 0));
		}

		mye::math::Vector3f Camera::Forward(void) const
		{
			return m_orientation.Rotate(mye::math::Vector3f(0, 0, 1));
		}

		void Camera::UpdateView(void)
		{
			m_viewMatrix = (const_cast<const Camera*>(this))->GetViewMatrix();
			m_viewMatrixUptodate = true;
		}

		mye::math::Matrix4f Camera::GetViewMatrix(void) const
		{

			mye::math::Matrix3f rot = RotationMatrix3(m_orientation);
			mye::math::Matrix3f rotT = rot.Transpose();
			mye::math::Vector3f trans = - (rotT * m_position);

			mye::math::Matrix4f viewMatrix(rotT);
			viewMatrix(0, 3) = trans.x();
			viewMatrix(1, 3) = trans.y();
			viewMatrix(2, 3) = trans.z();

			return viewMatrix;

		}

		mye::math::Matrix4f Camera::GetProjectionMatrix(void) const
		{

 			float yScale = 1.0f / mye::math::Tangent(m_fovY * 0.5f);
 			float xScale = yScale / m_aspectRatio;
 			float invDepth = 1.0f / (m_farClipDistance - m_nearClipDistance);
			float Q = m_farClipDistance / (m_farClipDistance - m_nearClipDistance);

			mye::math::Matrix4f projectionMatrix(0);

			projectionMatrix(0, 0) = xScale;
			projectionMatrix(1, 1) = yScale;
			projectionMatrix(2, 2) = Q;
			projectionMatrix(3, 2) = 1;
			projectionMatrix(2, 3) = - Q * m_nearClipDistance;

			return projectionMatrix;

		}

		const mye::math::Matrix4f& Camera::GetViewMatrix(void)
		{
			if (!m_viewMatrixUptodate)
			{
				UpdateView();
			}
			return m_viewMatrix;
		}

		const mye::math::Matrix4f& Camera::GetProjectionMatrix(void)
		{
			if (!m_projectionMatrixUptodate)
			{
				UpdateProjection();
			}
			return m_projectionMatrix;
		}

		void Camera::UpdateProjection(void)
		{
			m_projectionMatrix = ((const Camera*) this)->GetProjectionMatrix();
			m_projectionMatrixUptodate = true;
		}

	}

}