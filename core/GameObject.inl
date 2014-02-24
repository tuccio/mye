namespace mye
{

	namespace core
	{

		TransformComponent* GameObject::GetTransformComponent(void)
		{
			return m_transform;
		}

		ScriptComponent* GameObject::GetScriptComponent(void)
		{
			return m_script;
		}

		RenderComponent* GameObject::GetRenderComponent(void)
		{
			return m_render;
		}

		CameraComponent* GameObject::GetCameraComponent(void)
		{
			return m_camera;
		}

		GameObjectsManager* GameObject::GetOwner(void)
		{
			return m_owner;
		}

		GameObjectHandle GameObject::GetHandle(void)
		{
			return m_handle;
		}

		mye::math::AABBf GameObject::GetAABB(void)
		{
			return m_render->GetBounds().TransformAffine(m_transform->GetWorldMatrix());
		}

		ComponentIterator GameObject::begin(void)
		{
			return m_components.begin();
		}

		ComponentIterator GameObject::end(void)
		{
			return m_components.end();
		}

	}

}