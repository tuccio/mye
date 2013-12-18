#include "Component.h"

using namespace mye::core;
using namespace std;

const char* Component::Name(void)
{
	return NULL;
}

Component::Component(const string& name) :
	_name(name)
{
}

Component::~Component(void)
{

}

bool Component::IsNull(void) const
{
	return false;
}

const std::string& Component::GetName(void) const
{
	return _name;
}

Component& Component::operator= (const Component& component)
{
	return *this;
}