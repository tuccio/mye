#include "String.h"

#include <cstring>
#include <algorithm>

using namespace mye::core;

const String::Position String::Null = std::numeric_limits<size_t>::max();

String::String(void)
{
	m_string = nullptr;
	m_length = 0;
	m_size   = 0;
}

String::String(String &&string) :
	m_string(string.m_string),
	m_length(string.m_length),
	m_size(string.m_size)
{
	string.m_string = nullptr;
	string.m_length = 0;
	string.m_size   = 0;
}

String::String(size_t length) :
	m_string(nullptr),
	m_length(0),
	m_size(0)
{	
	Resize(length);
}

String::String(const char *string) :
	m_string(nullptr),
	m_length(0),
	m_size(0)
{

	if (string)
	{
		size_t length = strlen(string);
		Resize(length);
		memcpy(m_string, string, length);
		m_length = length;
	}
	
}

String::String(const String &string) :
	m_length(string.m_length)
{

	if (m_length > 0)
	{
		m_size   = m_length + 1;
		m_string = new char[m_size];
		memcpy(m_string, string.m_string, m_size);
	}
	else
	{
		m_string = nullptr;
		m_length = 0;
		m_size   = 0;
	}

}

String::~String(void)
{
	
	Clear();

}

void String::Resize(size_t length)
{

	if (length > 0)
	{

		if (length + 1 > m_size)
		{

			size_t oldSize  = m_size;
			char* oldString = m_string;

			m_size = length + 1;
			
			m_string = new char[m_size];

			if (oldString)
			{
				memcpy(m_string, oldString, oldSize);
				memset(m_string + oldSize, 0, m_size - oldSize);
				delete[] oldString;
			}
			else
			{
				memset(m_string, 0, m_size);
			}

		}
		else
		{
			m_length = length;
			memset(m_string + m_length, 0, m_size - m_length);
		}

	}
	else
	{

		m_length = 0;

		if (m_string)
		{
			memset(m_string, 0, m_size);
		}
		
	}

}

void String::Clear(void)
{

	if (m_string)
	{
		delete[] m_string;
	}

	m_string = nullptr;
	m_length = 0;
	m_size = 0;

}

String String::Flip(void) const
{
	String s(*this);
	s.FlipInPlace();
	return s;
}

void String::FlipInPlace(void)
{

	int hl = m_length / 2;

	for (int i = 0; i < hl; i++)
	{
		std::swap(m_string[i], m_string[m_length - 1 - i]);
	}

}

String String::Repeat(unsigned int n) const
{

	size_t nl = (m_length * n);
	String s(nl);

	char *d = s.m_string;

	for (int i = 0; i < n; i++)
	{
		memcpy(d, m_string, m_length);
		d += n;
	}

	s.m_length = nl;

	return s;

}

String::Iterator::Iterator(void)
{
}

String::Iterator::Iterator(char *s) :
	m_it(s)
{
}

String::Iterator::Iterator(const Iterator &it) :
	m_it(it.m_it)
{
}