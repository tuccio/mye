namespace mye
{

	namespace core
	{

		mye::core::Camera& SceneModule::Camera(void)
		{
			return *m_camera;
		}


		mye::core::Camera* SceneModule::GetCamera(void)
		{
			return m_camera;
		}

		const mye::core::Camera* SceneModule::GetCamera(void) const
		{
			return m_camera;
		}

		const void SceneModule::SetCamera(mye::core::Camera *camera)
		{
			m_camera = camera;
		}

	}

}