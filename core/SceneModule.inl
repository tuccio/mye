namespace mye
{

	namespace core
	{

		mye::core::CameraComponent& SceneModule::Camera(void)
		{
			return *m_camera;
		}


		mye::core::CameraComponent* SceneModule::GetCamera(void)
		{
			return m_camera;
		}

		const mye::core::CameraComponent* SceneModule::GetCamera(void) const
		{
			return m_camera;
		}

		void SceneModule::SetCamera(mye::core::CameraComponent *camera)
		{
			CameraComponent *old = m_camera;
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