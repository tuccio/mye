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

			void AddComponent(const std::string &name, const Component &component);
			Component* GetComponent(const std::string &name);
			void RemoveComponent(const std::string &name);

			const std::string& GetName(void) const;

			void Clear(void);

		private:

			friend class OpaqueObjectsManager<GameObject>;
			void SetName(const std::string &name);

			std::map<std::string, Component*> _components;
			std::string _name;

		};

		typedef OpaqueObjectHandle<GameObject> GameObjectHandle;
		typedef OpaqueObjectsManager<GameObject> GameObjectsManager;

	}

}