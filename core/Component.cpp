#include "Component.h"

using namespace mye::core;
using namespace std;

Component::Component(ComponentTypes type) :
	_type(type)
{
}

Component::~Component(void)
{

}

ComponentTypes Component::GetComponentType(void)
{
	return _type;
}