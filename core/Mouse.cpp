#include "Mouse.h"
#include <memory>

using namespace mye::core;
using namespace mye::math;

Mouse::Mouse(void) :
	m_wheelDelta(0),
	m_position(0),
	m_nextFramePosition(0),
	m_delta(0),
	m_firstUpdate(true),
	m_moveNextFrame(false)
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

void Mouse::MoveNextFrame(const Vector2 & position)
{

	m_nextFramePosition = position;
	m_moveNextFrame     = true;

}

void Mouse::NotifyMovement(void)
{

	if (m_moveNextFrame)
	{
		m_moveNextFrame = false;
		Move(m_nextFramePosition);
	}

}