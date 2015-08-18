#include "EventManager.h"

using namespace mye::core;

EventManager::EventManager(void) :
	m_currentQueue(0) { }

EventManager::~EventManager(void) { }

void EventManager::EnqueueEvent(IEvent * e)
{
	m_queueLock[m_currentQueue].lock();
 	m_events[m_currentQueue].push_back(e);
	m_queueLock[m_currentQueue].unlock();
}

void EventManager::EnqueueNextFrameEvent(IEvent * e)
{

	int nextQueue = m_currentQueue ^ 1;

	m_queueLock[nextQueue].lock();
	m_events[nextQueue].push_back(e);
	m_queueLock[nextQueue].unlock();

}

void EventManager::TriggerEvent(const IEvent * e)
{

	m_listenersLock.lock();
	auto eqr = m_listeners.equal_range(e->event);
	m_listenersLock.unlock();

	for (auto it = eqr.first; it != eqr.second; it++)
	{
		it->second->OnEvent(e);
	}

}

void EventManager::AddListener(EventType e, IEventListener * listener)
{
	m_listenersLock.lock();
	m_listeners.insert(std::make_pair(e, listener));
	m_listenersLock.unlock();
}

void EventManager::RemoveListener(EventType e, IEventListener * listener)
{

	m_listenersLock.lock();
	
	auto eqr = m_listeners.equal_range(e);

	for (auto it = eqr.first; it != eqr.second; it++)
	{

		if (it->second == listener)
		{
			m_listeners.erase(it);
			return;
		}

	}

	m_listenersLock.unlock();

}

void EventManager::Update(void)
{

	int currentQueue = m_currentQueue;

	m_listenersLock.lock();
	m_queueLock[currentQueue].lock();


	for (IEvent * e : m_events[currentQueue])
	{
		TriggerEvent(e);
		delete e;
	}

	m_events[currentQueue].clear();
	m_currentQueue ^= 1;

	m_queueLock[currentQueue].unlock();
	m_listenersLock.unlock();

}