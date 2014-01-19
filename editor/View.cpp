#include "View.h"

View::View(void)
{
}

View::~View(void)
{
}

Eigen::Vector2i View::GetPosition(void) const
{
	return m_position;
}

void View::SetPosition(const Eigen::Vector2i &position)
{
	m_position = position;
}

Eigen::Vector2i View::GetSize(void) const
{
	return m_size;
}

void View::SetSize(const Eigen::Vector2i &size)
{
	m_size = size;
}