#include "Object.h"

using namespace mye::luab;

Object::Object(void) :
	m_referenceType(ReferenceType::NONE)
{
}

Object::Object(StackReference stackReference) :
	m_referenceType(ReferenceType::STACK)
{
	m_reference.stack = stackReference;
}

Object::~Object(void)
{
}

int Object::GetLuaType(void)
{

	switch (m_referenceType)
	{

	case ReferenceType::STACK:
		return lua_type(m_reference.stack.L, m_reference.stack.index);
		break;

	case ReferenceType::NONE:
		return -1;
		break;

	}

}