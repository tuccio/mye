#include "Mouse.h"
#include <memory>

using namespace mye::core;

Mouse::Mouse(void)
{
	memset(m_keys, 0, sizeof(m_keys));
}

Mouse::~Mouse(void)
{

}