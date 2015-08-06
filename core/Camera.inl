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

		const mye::math::Matrix4 & Camera::GetViewMatrix(void) const
		{

			if (!m_viewMatrixUptodate)
			{
				UpdateView();
			}

			return m_viewMatrix;

		}

		const mye::math::Matrix4 & Camera::GetProjectionMatrix(void) const
		{

			if (!m_projectionMatrixUptodate)
			{
				UpdateProjection();
			}

			return m_projectionMatrix;

		}

		const mye::math::Frustum & Camera::GetFrustum(void) const
		{

			if (!m_frustumUptodate)
			{
				UpdateFrustum();
			}

			return m_frustum;

		}
	}

}