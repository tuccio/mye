namespace mye
{

	namespace core
	{

		mye::math::Matrix4f Camera::GetViewMatrix(void) const
		{

			mye::math::Matrix4f viewMatrix = mye::math::RotationMatrix4(m_orientation);

			viewMatrix(0, 3) = - mye::math::Vector3f(viewMatrix(0, 0),
				viewMatrix(0, 1),
				viewMatrix(0, 2)).Dot(m_position);

			viewMatrix(1, 3) = - mye::math::Vector3f(viewMatrix(1, 0),
				viewMatrix(1, 1),
				viewMatrix(1, 2)).Dot(m_position);

			viewMatrix(2, 3) = - mye::math::Vector3f(viewMatrix(2, 0),
				viewMatrix(2, 1),
				viewMatrix(2, 2)).Dot(m_position);

			return viewMatrix;

		}

		mye::math::Matrix4f Camera::GetProjectionMatrix(void) const
		{

			mye::math::Matrix4f projectionMatrix;

			float yScale = 1.0f / mye::math::Tangent(m_fovY * 0.5f);
			float xScale = yScale / m_aspectRatio;
			float invDepth = 1.0f / (m_farClipDistance - m_nearClipDistance);

			projectionMatrix(0, 0) = xScale;
			projectionMatrix(1, 1) = yScale;
			projectionMatrix(2, 2) = m_farClipDistance * invDepth;
			projectionMatrix(2, 3) = - m_nearClipDistance * m_farClipDistance * invDepth;
			projectionMatrix(3, 2) = 1;

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

	}

}