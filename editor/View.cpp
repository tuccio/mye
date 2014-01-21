#include "View.h"

View::View(void)
{
}

View::~View(void)
{
}

mye::math::Vector2i View::GetPosition(void) const
{
	return m_position;
}

void View::SetPosition(const mye::math::Vector2i &position)
{
	m_position = position;
}

mye::math::Vector2i View::GetSize(void) const
{
	return m_size;
}

void View::SetSize(const mye::math::Vector2i &size)
{
	m_size = size;
}