#include "EventManager.h"

using namespace mye::core;

EventManager::EventManager(void) :
	m_currentQueue(0) { }

EventManager::~EventManager(void) { }

void EventManager::EnqueueEvent(IEvent * e)
{
	m_events[m_currentQueue].push_back(e);
}

void EventManager::EnqueueNextFrameEvent(IEvent * e)
{
	m_events[m_currentQueue ^ 1].push_back(e);
}

void EventManager::TriggerEvent(const IEvent * e)
{
	
	auto eqr = m_listeners.equal_range(e->event);

	for (auto it = eqr.first; it != eqr.second; it++)
	{
		it->second->OnEvent(e);
	}

}

void EventManager::AddListener(EventType e, IEventListener * listener)
{
	m_listeners.insert(std::make_pair(e, listener));
}

void EventManager::RemoveListener(EventType e, IEventListener * listener)
{
	
	auto eqr = m_listeners.equal_range(e);

	for (auto it = eqr.first; it != eqr.second; it++)
	{

		if (it->second == listener)
		{
			m_listeners.erase(it);
			return;
		}

	}

}

void EventManager::Update(void)
{

	int currentQueue = m_currentQueue;

	m_currentQueue ^= 1;

	for (IEvent * e : m_events[currentQueue])
	{
		TriggerEvent(e);
		delete e;
	}

	m_events[currentQueue].clear();

	

}