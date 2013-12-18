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

void VariableComponent::Register(const std::string &name, double initValue)
{
	
	auto it = _info.find(name);

	if (it == _info.end())
	{
		_info.insert(pair<string, double>(name, initValue));

	}

}

double VariableComponent::Get(const std::string &name)
{

	auto it = _info.find(name);

	if (it != _info.end())
	{
		return it->second;
	}
	else
	{
		throw;
	}

}

void VariableComponent::Set(const std::string &name, double value)
{

	auto it = _info.find(name);

	if (it != _info.end())
	{
		it->second = value;
	}
	else
	{
		throw;
	}

}

void VariableComponent::Remove(const std::string &name)
{

	auto it = _info.find(name);

	if (it != _info.end())
	{
		_info.erase(it);
	}
	else
	{
		throw;
	}

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