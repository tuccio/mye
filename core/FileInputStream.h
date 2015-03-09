#pragma once

#include "InputStream.h"

#include <cstdio>

namespace mye
{

	namespace core
	{

		class FileInputStream :
			public InputStream
		{

		public:

			FileInputStream(const String & file);
			FileInputStream(void);

			~FileInputStream(void);

			inline bool IsOver(void) const;
			inline bool IsOpen(void) const;

			FileInputStream & operator>> (String & s);

			size_t Read(void * data, size_t size);

		private:

			void _Bufferize(void);

			FILE * m_file;

			unsigned char m_buffer[BUFSIZ];

			unsigned short m_begin;
			unsigned short m_end;

			bool m_eof;

		};

	}

}

#include "FileInputStream.inl"