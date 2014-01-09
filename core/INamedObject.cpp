#include "INamedObject.h"

using namespace mye::core;

INamedObject::INamedObject(void)
{
}

INamedObject::INamedObject(const std::string &name) :
	m_name(name)
{
}

INamedObject::~INamedObject(void)
{
}

const std::string& INamedObject::GetName(void) const
{
	return m_name;
}

void INamedObject::SetName(const std::string &name)
{
	m_name = name;
}