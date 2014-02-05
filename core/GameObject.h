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

namespace mye
{

	namespace core
	{

		class GameObject :
			public INamedObject
		{

		public:

			typedef std::vector<GameObject*> ChildrenList;

			GameObject(void);
			GameObject(const std::string &name);

			~GameObject(void);

			Component* AddComponent(const Component &component);
			Component* GetComponent(const std::string &name);
			void RemoveComponent(const std::string &name);

			inline TransformComponent* GetTransformComponent(void);
			inline ScriptComponent* GetScriptComponent(void);
			inline RenderComponent* GetRenderComponent(void);

			inline GameObjectsManager* GetOwner(void);
			inline GameObjectHandle GetHandle(void);

			void Update(FloatSeconds dt);

			void Clear(void);

			MYE_DECLARE_POOL_ALLOCATOR(GameObject)

		private:

			typedef std::unordered_map<std::string, Component*> ComponentsList;

			friend class GameObjectsManager;

			void OnCreation(GameObjectsManager *owner,
				const GameObjectHandle &handle);
			void OnDestruction(void);

			ComponentsList m_components;
			std::string m_name;

			GameObjectHandle m_handle;
			GameObjectsManager *m_owner;

			TransformComponent *m_transform;
			ScriptComponent *m_script;
			RenderComponent *m_render;

		};

		

	}

}

#include "GameObject.inl"