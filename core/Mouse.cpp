#include "Mouse.h"
#include <memory>

using namespace mye::core;

Mouse::Mouse(void)
{
	memset(m_keys, 0, sizeof(m_keys));
	m_wheelDelta = 0;
}

Mouse::~Mouse(void)
{

}