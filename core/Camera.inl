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

		const mye::math::Matrix4 & Camera::GetViewMatrix(void)
		{

			if (!m_viewMatrixUptodate)
			{
				UpdateView();
			}

			return m_viewMatrix;
		}

		const mye::math::Matrix4 & Camera::GetProjectionMatrix(void)
		{

			if (!m_projectionMatrixUptodate)
			{
				UpdateProjection();
			}

			return m_projectionMatrix;

		}

		void Camera::UpdateProjection(void)
		{
			m_projectionMatrix = const_cast<const Camera *>(this)->GetProjectionMatrix();
			m_projectionMatrixUptodate = true;
		}

		mye::math::Frustum Camera::GetFrustum(void) const
		{
			return (m_frustumUptodate ? m_frustum :
				mye::math::Frustum(m_position,
				                   Forward(), Up(), Right(),
				                   m_nearClipDistance, m_farClipDistance,
				                   GetFovX(), GetFovY())
				);
		}

		const mye::math::Frustum & Camera::GetFrustum(void)
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