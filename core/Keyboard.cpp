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
