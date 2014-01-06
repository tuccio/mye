#pragma once

#include <map>
#include <string>

#include "Component.h"
#include "INamedObject.h"
#include "OpaqueObjectsManager.h"

namespace mye
{

	namespace core
	{

		class GameObject :
			public INamedObject
		{

		public:

			GameObject(void);
			GameObject(const std::string &name);

			~GameObject(void);

			void AddComponent(const std::string &name, const Component &component);
			Component* GetComponent(const std::string &name);
			void RemoveComponent(const std::string &name);

			void Clear(void);

		private:

			friend class OpaqueObjectsManager<GameObject>;

			std::map<std::string, Component*> _components;
			std::string _name;

		};

		typedef OpaqueObjectHandle<GameObject> GameObjectHandle;
		typedef OpaqueObjectsManager<GameObject> GameObjectsManager;

	}

}