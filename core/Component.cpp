#include "Component.h"

using namespace mye::core;
using namespace std;

Component::Component(ComponentTypes type) :
	m_type(type)
{
}

Component::~Component(void)
{

}

ComponentTypes Component::GetComponentType(void)
{
	return m_type;
}