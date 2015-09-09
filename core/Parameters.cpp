#include "Parameters.h"

#include <mye/core/Debug.h>
#include <mye/core/Utils.h>

using namespace mye::core;

Parameters::Parameters(void)
{
}

Parameters::Parameters(std::initializer_list<Parameter> initializerList)
{
	for (const Parameter &p : initializerList)
	{
		m_map[p.key] = p.value;
	}
}

Parameters::Parameters(const std::unordered_map<String, String> map) :
	m_map(map)
{
}


Parameters::~Parameters(void)
{
}

String Parameters::GetString(const String & key) const
{

	String s;
	auto it = m_map.find(key);

	if (it != m_map.end())
	{
		s = it->second;
	}

	return s;

}

int Parameters::GetInteger(const String & key) const
{

	String s = GetString(key);

	int value = 0;

	if (!s.IsEmpty())
	{
		value = ParseType<int>(s);
	}

	return value;

}

float Parameters::GetFloat(const String & key) const
{

	String s = GetString(key);

	float value = 0;

	if (!s.IsEmpty())
	{
		value = ParseType<float>(s);
	}

	return value;

}

mye::math::Real Parameters::GetReal(const String & key) const
{

	String s = GetString(key);

	mye::math::Real value = 0;

	if (!s.IsEmpty())
	{
		value = ParseType<mye::math::Real>(s);
	}

	return value;

}

bool Parameters::GetBool(const String & key) const
{

	String s = GetString(key);

	bool value = false;

	if (!s.IsEmpty())
	{
		value = ParseType<bool>(s);
	}

	return value;

}

void* Parameters::GetPointer(const String & key) const
{

	String s = GetString(key);

	void *value = nullptr;

	if (!s.IsEmpty())
	{
		value = StringToPointer(s);
	}

	return value;

}

void Parameters::Add(const String & key, const String & value)
{
	m_map[key] = value;
}

void Parameters::Remove(const String & key)
{

	auto it = m_map.find(key);

	if (it != m_map.end())
	{
		m_map.erase(it);
	}

}

bool Parameters::Contains(const String & key) const
{

	bool found = false;
	auto it = m_map.find(key);

	if (it != m_map.end())
	{
		found = true;
	}

	return found;

}

void Parameters::Clear(void)
{
	m_map.clear();
}

size_t Parameters::Size(void) const
{
	return m_map.size();
}

String & Parameters::operator[] (const String & key)
{
	return m_map[key];
}

Parameters::Iterator Parameters::begin(void) const
{
	return m_map.cbegin();
}

Parameters::Iterator Parameters::end(void) const
{
	return m_map.cend();
}