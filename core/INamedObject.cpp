#include "INamedObject.h"

using namespace mye::core;

INamedObject::INamedObject(void)
{
}

INamedObject::INamedObject(const String & name) :
	m_name(name)
{
}

INamedObject::~INamedObject(void)
{
}

const String & INamedObject::GetName(void) const
{
	return m_name;
}

void INamedObject::SetName(const String & name)
{
	m_name = name;
}