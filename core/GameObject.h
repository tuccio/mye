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

		class GameObjectListener;

		class GameObject :
			public INamedObject
		{

		public:

			MYE_DECLARE_POOL_ALLOCATOR(GameObject)

			GameObject(void);
			GameObject(const String &name);

			~GameObject(void);

			void AddListener(GameObjectListener *listener);
			void RemoveListener(GameObjectListener *listener);

			Component * AddComponent(const Component &component);
			Component * AddComponent(Component * component);

			Component * GetComponent(const String &name);
			     void   RemoveComponent(const String &name);

			inline TransformComponent * GetTransformComponent(void);
			inline    RenderComponent * GetRenderComponent(void);
			inline    CameraComponent * GetCameraComponent(void);
			inline BehaviourComponent * GetBehaviourComponent(void);
			inline RigidBodyComponent * GetRigidBodyComponent(void);
			inline    Text2DComponent * GetText2DComponent(void);
			inline     LightComponent * GetLightComponent(void);

			inline    mye::math::AABB   GetAABB(void);

			inline GameObjectsManager * GetOwner(void);
			inline   GameObjectHandle   GetHandle(void);

			inline       const String & GetEntityType(void) const;

			void Clear(void);

			inline ComponentIterator begin(void);
			inline ComponentIterator end(void);

		private:

			friend class GameObjectsManager;
			friend class GameObjectsModule;

			void OnCreation(GameObjectsManager * owner, const GameObjectHandle & handle);

			void OnDestruction(void);
			
			    ComponentsList   m_components;

			  GameObjectHandle   m_handle;
			GameObjectsManager * m_owner;

			TransformComponent   m_transform;
			BehaviourComponent * m_behaviour;

			String m_entity;
			std::list<GameObjectListener*> m_listeners;

			bool m_deleteFlag : 1;


		};

		class GameObjectListener
		{

		public:

			virtual inline void OnGameObjectCreation(GameObject *gameObject) { };
			virtual inline void OnGameObjectDestruction(GameObject *gameObject) { };

			virtual inline void OnComponentAddition(GameObject *gameObject, Component *component) { };
			virtual inline void OnComponentRemoval(GameObject *gameObject, Component *component) { };

		};

	}

}

#include "GameObject.inl"