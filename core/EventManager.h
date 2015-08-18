#pragma once

#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include <unordered_map>
#include <vector>

#include "ThreadConfig.h"
#include "Singleton.h"

#define __MYE_EVENT_MANAGER_CALL_LOOP(r, Data, Element)   em->BOOST_PP_SEQ_ELEM(1, Data)(Element, BOOST_PP_SEQ_ELEM(0, Data));
#define __MYE_EVENT_MANAGER_CALL(Listener, Function, ...) BOOST_PP_LIST_FOR_EACH(__MYE_EVENT_MANAGER_CALL_LOOP, (Listener)(Function), BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

#define MYE_EVENT_MANAGER_ENQUEUE(Event, ...)            BOOST_PP_EXPAND(mye::core::EventManager::GetSingleton().EnqueueEvent(new Event(__VA_ARGS__)))
#define MYE_EVENT_MANAGER_ENQUEUE_NEXT_FRAME(Event, ...) BOOST_PP_EXPAND(mye::core::EventManager::GetSingleton().EnqueueNextFrameEvent(new Event(__VA_ARGS__)))
#define MYE_EVENT_MANAGER_TRIGGER(Event, ...)            BOOST_PP_EXPAND(mye::core::EventManager::GetSingleton().TriggerEvent(&Event(__VA_ARGS__)))

#define MYE_EVENT_MANAGER_ADD_LISTENER(Listener, ...) { EventManager * em = mye::core::EventManager::GetSingletonPointer(); __MYE_EVENT_MANAGER_CALL(Listener, AddListener, __VA_ARGS__) }
#define MYE_EVENT_MANAGER_REMOVE_LISTENER(Listener, ...) { EventManager * em = mye::core::EventManager::GetSingletonPointer(); __MYE_EVENT_MANAGER_CALL(Listener, RemoveListener, __VA_ARGS__) }

namespace mye
{

	namespace core
	{

		enum class EventType
		{

			GAME_OBJECT_CREATE,
			GAME_OBJECT_DESTROY,
			GAME_OBJECT_MOVE,
			GAME_OBJECT_ADD_COMPONENT,
			GAME_OBJECT_REMOVE_COMPONENT,

			KEYBOARD_KEY_PRESS,
			KEYBOARD_KEY_RELEASE,
			KEYBOARD_KEY_HOLD,

			MOUSE_KEY_PRESS,
			MOUSE_KEY_RELEASE,
			MOUSE_KEY_HOLD,
			MOUSE_MOVE,

			RENDERER_VARIABLE_CHANGE

		};

		struct IEvent
		{

			IEvent(EventType event) :
				event(event) { }

			virtual ~IEvent(void) { }

			EventType event;

		};

		struct IEventListener
		{

			virtual void OnEvent(const IEvent * e) = 0;

		};

		class EventManager :
			public Singleton<EventManager>
		{

		public:

			EventManager(void);
			~EventManager(void);

			void EnqueueEvent(IEvent * e);
			void EnqueueNextFrameEvent(IEvent * e);

			void TriggerEvent(const IEvent * e);

			void AddListener(EventType e, IEventListener * listener);
			void RemoveListener(EventType e, IEventListener * listener);

			void Update(void);

		private:

			typedef std::unordered_multimap<EventType, IEventListener*> ListenersMap;
			typedef std::vector<IEvent*> EventsQueue;

			ListenersMap m_listeners;
			EventsQueue  m_events[2];

			short        m_currentQueue;

			mutable boost::recursive_mutex m_queueLock[2];
			mutable boost::recursive_mutex m_listenersLock;

		};

	}

}