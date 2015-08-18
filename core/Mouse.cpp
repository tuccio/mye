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

void Mouse::NotifyHeldKeys(void)
{

	for (auto & keyPressed : m_pressedKeys)
	{
		MYE_EVENT_MANAGER_ENQUEUE_NEXT_FRAME(MouseEventKeyHold, keyPressed.key, keyPressed.timer.GetElapsedTime());
	}
}