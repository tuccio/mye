#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include "AlignedAllocator.h"
#include "Component.h"
#include "Components.h"
#include "GameObjectsManager.h"
#include "INamedObject.h"
#include "Time.h"
#include "EventManager.h"

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

		class __MYE_ALIGNED(16) GameObject :
			public INamedObject
		{

		public:

			__MYE_DECLARE_ALIGNED_16
			//MYE_DECLARE_POOL_ALLOCATOR(GameObject)

			GameObject(void);
			GameObject(const String &name);

			~GameObject(void);

			Component * AddComponent(const Component & component);
			Component * AddComponent(Component * component);

			Component * GetComponent(const String & name);
			     void   RemoveComponent(const String & name);

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
			
			TransformComponent   m_transform;

			ComponentsList       m_components;

			GameObjectHandle     m_handle;
			GameObjectsManager * m_owner;

			BehaviourComponent * m_behaviour;

			String m_entity;

			bool m_deleteFlag : 1;


		};

		struct GameObjectEventCreate :
			IEvent
		{

			GameObjectEventCreate(GameObject * object) :
				IEvent(EventType::GAME_OBJECT_CREATE),
				object(object) { }

			GameObject * object;

		};

		struct GameObjectEventDestroy :
			IEvent
		{

			GameObjectEventDestroy(GameObject * object) :
				IEvent(EventType::GAME_OBJECT_DESTROY),
				object(object) { }

			GameObject * object;

		};

		struct __MYE_ALIGNED(16) GameObjectEventMove :
			IEvent
		{

			__MYE_DECLARE_ALIGNED_16

			GameObjectEventMove(GameObject * object,
			                    const mye::math::Matrix4 & from,
			                    const mye::math::Matrix4 & to) :
				IEvent(EventType::GAME_OBJECT_MOVE),
				object(object),
				from(from),
				to(to) { }

			mye::math::Matrix4 from;
			mye::math::Matrix4 to;

			GameObject * object;


		};

		struct GameObjectEventFree :
			IEvent
		{

			GameObjectEventFree(GameObject * object) :
				IEvent(EventType::GAME_OBJECT_FREE),
				object(object) { }

			GameObject * object;

		};

		struct GameObjectEventAddComponent :
			IEvent
		{

			GameObjectEventAddComponent(GameObject * object, Component * component) :
				IEvent(EventType::GAME_OBJECT_ADD_COMPONENT),
				object(object),
				component(component) { }

			GameObject * object;
			Component  * component;

		};

		struct GameObjectEventRemoveComponent :
			IEvent
		{

			GameObjectEventRemoveComponent(GameObject * object, Component * component) :
				IEvent(EventType::GAME_OBJECT_REMOVE_COMPONENT),
				object(object),
				component(component) { }

			GameObject * object;
			Component  * component;

		};

	}

}

#include "GameObject.inl"