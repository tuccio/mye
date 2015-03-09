#include "FileInputStream.h"

#include <mye/math/Math.h>
#include <vector>

using namespace mye::core;

FileInputStream::FileInputStream(const String & file) :
	m_begin(0),
	m_end(0),
	m_eof(false)
{
	m_file = fopen(file.CString(), "rb");
}

FileInputStream::FileInputStream(void) :
	m_file(nullptr),
	m_begin(0),
	m_end(0),
	m_eof(false)
{
}

FileInputStream::~FileInputStream(void)
{
}

FileInputStream & FileInputStream::operator>> (String & s)
{

	if (m_file)
	{
		
		bool reading = false;

		std::vector<char> v;

		if (m_begin == m_end &&
			!m_eof)
		{
			_Bufferize();
		}

		if (m_begin != m_end)
		{
			
			while (!reading)
			{

				if (m_begin == m_end)
				{

					_Bufferize();

					if (m_begin == m_end)
					{
						break;
					}

				}

				if (m_buffer[m_begin] != ' '  &&
					m_buffer[m_begin] != '\n' &&
					m_buffer[m_begin] != '\r' &&
					m_buffer[m_begin] != '\t' &&
					m_buffer[m_begin] != '\0')
				{
					reading = true;
				}
				else
				{
					m_begin++;
				}
				

			}

			while (reading)
			{

				if (m_begin == m_end)
				{

					_Bufferize();

					if (m_begin == m_end)
					{
						reading = false;
						break;
					}

				}

				if (m_buffer[m_begin] != ' '  &&
					m_buffer[m_begin] != '\n' &&
					m_buffer[m_begin] != '\r' &&
					m_buffer[m_begin] != '\t' &&
					m_buffer[m_begin] != '\0')
				{
					v.push_back(m_buffer[m_begin++]);
				}
				else
				{
					reading = false;
				}

			}

		}

		v.push_back('\0');
		s = &v[0];

	}

	return *this;

}

size_t FileInputStream::Read(void * data, size_t size)
{
	
	size_t read = 0;
	size_t left = size;

	bool done = false;

	do 
	{

		if (m_begin == m_end &&
			!m_eof)
		{
			
			_Bufferize();

			if (m_begin == m_end)
			{
				done = true;
			}

		}
		else
		{

			size_t available = m_end - m_begin;

			size_t r = mye::math::Min(available, left);

			memcpy((char*) data + read, (char*) m_buffer + m_begin, r);

			left -= r;
			read += r;

			if (left == 0)
			{
				done = true;
			}

		}

	} while (!done);

	return read;

}

void FileInputStream::_Bufferize(void)
{

	m_begin = 0;
	m_end = fread(m_buffer, 1, BUFSIZ, m_file);

	m_eof = (m_end < BUFSIZ);

}