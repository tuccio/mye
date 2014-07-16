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