#include "Mouse.h"
#include <memory>

using namespace mye::core;

Mouse::Mouse(void) :
	m_wheelDelta(0),
	m_position(0, 0),
	m_delta(0, 0),
	m_firstUpdate(true)
{
	memset(m_keys, 0, sizeof(m_keys));
}

Mouse::~Mouse(void)
{

}

void Mouse::AddListener(MouseListener * listener)
{
	m_listeners.push_back(listener);
}

void Mouse::RemoveListener(MouseListener * listener)
{

	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);

	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
	}

}

void Mouse::NotifyHeldKeys(void)
{

	for (auto & keyPressed : m_pressedKeys)
	{
		for (auto listener : m_listeners)
		{
			listener->OnMouseKeyHold(keyPressed.key, keyPressed.timer.GetElapsedTime());
		}
	}
}