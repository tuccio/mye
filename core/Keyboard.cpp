#include "Keyboard.h"

#include <memory>

using namespace mye::core;

Keyboard::Keyboard(void)
{
	memset(m_keys, 0, sizeof(m_keys));
}


Keyboard::~Keyboard(void)
{
}

void Keyboard::AddListener(KeyboardListener *listener)
{
	m_listeners.push_back(listener);
}

void Keyboard::RemoveListener(KeyboardListener *listener)
{

	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);

	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
	}

}

void KeyboardListener::OnKeyPress(KeyboardVK key)
{
}

 void KeyboardListener::OnKeyRelease(KeyboardVK key)
 {
 }