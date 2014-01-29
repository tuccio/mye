#include "Component.h"

using namespace mye::core;
using namespace std;

Component::Component(ComponentTypes type,
					 const std::string &name) :
	INamedObject(name),
	m_type(type)
{
}

Component::~Component(void)
{

}

ComponentTypes Component::GetComponentType(void) const
{
	return m_type;
}