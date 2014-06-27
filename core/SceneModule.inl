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

		void SceneModule::SetCamera(mye::core::Camera *camera)
		{
			mye::core::Camera *old = m_camera;
			m_camera = camera;
			for (SceneCameraListener *listener : m_cameraListeners)
			{
				listener->OnCameraChange(old, camera);
			}
		}

		void SceneModule::MoveGameObject(
			const GameObjectHandle &hObj,
			const mye::math::AABB &oldAABB)
		{
			GameObjectUpdate update = { hObj, oldAABB };
			m_movedObjects.push_back(update);
		}

	}

}