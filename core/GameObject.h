#pragma once

#include <map>
#include <string>

#include "Component.h"
#include "OpaqueObjectsManager.h"

namespace mye
{

	namespace core
	{

		class GameObject
		{

		public:

			GameObject(void);
			~GameObject(void);

			void AddComponent(const Component &component);

			Component* GetComponent(const std::string &name);
			
			template <typename T>
			T* GetComponent(void);

			void RemoveComponent(const std::string &name);

			void Destroy(void);

			void Clear(void);

		private:

			std::map<std::string, Component*> _components;

		};

		template <typename T>
		T* GameObject::GetComponent(void)
		{
			return static_cast<T*>(GetComponent(T::Name()));
		}

		typedef OpaqueObjectHandle<GameObject> GameObjectHandle;
		typedef OpaqueObjectsManager<GameObject> GameObjectsManager;

	}

}