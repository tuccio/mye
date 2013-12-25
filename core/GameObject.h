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
			GameObject(const std::string &name);
			~GameObject(void);

			void AddComponent(const Component &component);

			Component* GetComponent(const std::string &name);
			
			template <typename T>
			T* GetComponent(void);

			const std::string& GetName(void) const;			

			void SetType(const std::string &name);
			const std::string& GetType(void) const;

			void RemoveComponent(const std::string &name);

			void Destroy(void);

			void Clear(void);

		private:

			friend class OpaqueObjectsManager<GameObject>;
			void SetName(const std::string &name);

			std::map<std::string, Component*> _components;
			std::string _type;
			std::string _name;

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