#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include "Component.h"
#include "INamedObject.h"
#include "OpaqueObjectsManager.h"
#include "Time.h"

namespace mye
{

	namespace core
	{

		class GameObject;

		typedef OpaqueObjectHandle<GameObject> GameObjectHandle;
		typedef OpaqueObjectsManager<GameObject> GameObjectsManager;

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

			GameObjectsManager* GetOwner(void);
			GameObjectHandle GetHandle(void);

			void Update(FloatSeconds dt);

			void Clear(void);

		private:

			typedef std::unordered_map<std::string, Component*> ComponentsList;

			friend class OpaqueObjectsManager<GameObject>;

			void OnCreation(GameObjectsManager *owner,
				const GameObjectHandle &handle);
			void OnDestruction(void);

			ComponentsList m_components;
			std::string m_name;

			GameObjectHandle m_handle;
			GameObjectsManager *m_owner;

		};

		

	}

}