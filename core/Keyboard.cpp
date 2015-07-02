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

void Keyboard::NotifyHeldKeys(void)
{

	for (auto & keyPressed : m_pressedKeys)
	{
		MYE_EVENT_MANAGER_ENQUEUE(KeyboardEventKeyHold, keyPressed.key, keyPressed.timer.GetElapsedTime());
	}
}