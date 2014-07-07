#include "Component.h"

using namespace mye::core;
using namespace std;

Component::Component(ComponentTypes type,
					 const String &name) :
	INamedObject(name),
	m_type(type),
	m_owner(nullptr)
{
}

Component::~Component(void)
{

}

ComponentTypes Component::GetComponentType(void) const
{
	return m_type;
}

void Component::OnAttach(GameObject *go)
{
	m_owner = go;
}

void Component::OnDetach(void)
{
	m_owner = nullptr;
}