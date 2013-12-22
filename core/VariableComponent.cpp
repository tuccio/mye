#include "VariableComponent.h"

using namespace mye::core;
using namespace std;

const char* VariableComponent::Name(void)
{
	return "variable";
}

VariableComponent::VariableComponent(void) :
	Component(Name())
{
}


VariableComponent::~VariableComponent(void)
{
}

bool VariableComponent::Contains(const std::string &name) const
{
	return _info.find(name) != _info.end();
}

Variable& VariableComponent::operator[] (const std::string &name)
{
	
	auto it = _info.find(name);

	if (it == _info.end())
	{
		return (_info[name] = Variable());
	}

	return it->second;

}

const Variable& VariableComponent::operator[] (const std::string &name) const
{

	auto it = _info.find(name);

	if (it == _info.end())
	{
		throw;
	}

	return it->second;

}

void VariableComponent::Clear(void)
{
	_info.clear();
}

Cloneable* VariableComponent::Clone(void) const
{
	VariableComponent *nc = new VariableComponent;
	*nc = *this;
	return nc;
}

Component& VariableComponent::operator= (const Component& component)
{
	
	Clear();

	const VariableComponent &nc = static_cast<const VariableComponent&>(component);

	_info = nc._info;

	return *this;

}