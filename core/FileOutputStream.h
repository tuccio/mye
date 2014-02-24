#pragma once

#include <cstdio>

#include "OutputStream.h"

namespace mye
{

	namespace core
	{

		class FileOutputStream :
			public OutputStream
		{

		public:

			FileOutputStream(void);
			FileOutputStream(FILE *file);
			FileOutputStream(const String &file);

			~FileOutputStream(void);

			inline bool IsOpen(void) const;
			inline void Close(void);

			inline FileOutputStream& operator<< (const String &s);

			inline FileOutputStream& operator<< (int x);
			inline FileOutputStream& operator<< (unsigned int x);
			inline FileOutputStream& operator<< (float x);
			inline FileOutputStream& operator<< (double x);

			size_t Write(const void *data, size_t size);

			inline operator bool () const;

		private:

			FILE *m_file;

		};

		extern FileOutputStream StandardOutput;

	}

}

#include "FileOutputStream.inl"