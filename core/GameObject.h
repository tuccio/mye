#pragma once

#include <map>
#include <string>
#include <vector>

#include "Component.h"
#include "INamedObject.h"
#include "OpaqueObjectsManager.h"

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

			void AddComponent(const std::string &name, const Component &component);
			Component* GetComponent(const std::string &name);
			void RemoveComponent(const std::string &name);

			GameObject* GetParent(void);
			void SetParent(GameObject *object);

			const ChildrenList& GetChildren(void) const;

			GameObjectsManager* GetOwner(void);
			GameObjectHandle GetHandle(void);

			void Clear(void);

		private:

			friend class OpaqueObjectsManager<GameObject>;

			void OnCreation(GameObjectsManager *owner,
				const GameObjectHandle &handle);
			void OnDestruction(void);

			void OnParentDestruction(void);
			void OnChildDestruction(GameObject *child);

			void SetOwner(GameObjectsManager *manager);
			void SetHandle(const GameObjectHandle &handle);

			std::map<std::string, Component*> _components;
			std::string _name;

			GameObject *_parent;
			ChildrenList _children;

			GameObjectHandle _handle;
			GameObjectsManager *_owner;

		};

		

	}

}