namespace mye
{

	namespace core
	{

		TransformComponent* GameObject::GetTransformComponent(void)
		{
			return &m_transform;
		}

		BehaviourComponent* GameObject::GetBehaviourComponent(void)
		{
			return m_behaviour;
		}

		RigidBodyComponent* GameObject::GetRigidBodyComponent(void)
		{
			return static_cast<RigidBodyComponent*>(GetComponent("rigidbody"));
		}

		RenderComponent* GameObject::GetRenderComponent(void)
		{
			return static_cast<RenderComponent*>(GetComponent("render"));
		}

		Text2DComponent* GameObject::GetText2DComponent(void)
		{
			return static_cast<Text2DComponent*>(GetComponent("text2d"));
		}

		CameraComponent* GameObject::GetCameraComponent(void)
		{
			return static_cast<CameraComponent*>(GetComponent("camera"));
		}

		LightComponent* GameObject::GetLightComponent(void)
		{
			return static_cast<LightComponent*>(GetComponent("light"));
		}

		GameObjectsManager* GameObject::GetOwner(void)
		{
			return m_owner;
		}

		GameObjectHandle GameObject::GetHandle(void)
		{
			return m_handle;
		}

		const String& GameObject::GetEntityType(void) const
		{
			return m_entity;
		}

		mye::math::AABB GameObject::GetAABB(void)
		{
			return GetRenderComponent()->GetBounds().TransformAffine(m_transform.GetWorldMatrix());
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