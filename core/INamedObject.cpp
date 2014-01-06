#include "INamedObject.h"

using namespace mye::core;

INamedObject::INamedObject(void)
{
}

INamedObject::INamedObject(const std::string &name) :
	_name(name)
{
}

INamedObject::~INamedObject(void)
{
}

const std::string& INamedObject::GetName(void) const
{
	return _name;
}

void INamedObject::SetName(const std::string &name)
{
	_name = name;
}