#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include "Component.h"
#include "Components.h"
#include "GameObjectsManager.h"
#include "INamedObject.h"
#include "Time.h"

#include "PoolAllocator.h"

#include "String.h"

#include <mye/math/Geometry.h>

namespace mye
{

	namespace core
	{

		typedef std::unordered_map<String, Component*> ComponentsList;
		typedef std::unordered_map<String, Component*>::iterator ComponentIterator;

		class GameObject :
			public INamedObject
		{

		public:

			GameObject(void);
			GameObject(const String &name);

			~GameObject(void);

			Component* AddComponent(const Component &component);
			Component* GetComponent(const String &name);
			void RemoveComponent(const String &name);

			inline TransformComponent* GetTransformComponent(void);
			inline ScriptComponent* GetScriptComponent(void);
			inline RenderComponent* GetRenderComponent(void);
			inline CameraComponent* GetCameraComponent(void);

			inline mye::math::AABBf GetAABB(void);

			inline GameObjectsManager* GetOwner(void);
			inline GameObjectHandle GetHandle(void);

			void Update(FloatSeconds dt);

			void Clear(void);

			inline ComponentIterator begin(void);
			inline ComponentIterator end(void);

			MYE_DECLARE_POOL_ALLOCATOR(GameObject)

		private:

			friend class GameObjectsManager;

			void OnCreation(GameObjectsManager *owner,
				const GameObjectHandle &handle);

			void OnDestruction(void);

			ComponentsList m_components;

			GameObjectHandle m_handle;
			GameObjectsManager *m_owner;

			TransformComponent *m_transform;
			ScriptComponent *m_script;
			RenderComponent *m_render;
			CameraComponent *m_camera;

		};

		

	}

}

#include "GameObject.inl"