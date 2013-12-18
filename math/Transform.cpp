#include "Transform.h"

using namespace mye::math;

Transform::Transform(void)
{
}


Transform::~Transform(void)
{
}

const Vector3f& Transform::GetPosition(void) const
{
	return _position;
}
void Transform::SetPosition(const Vector3f &p)
{
	_position = p;
}

Vector3f& Transform::Position(void)
{
	return _position;
}