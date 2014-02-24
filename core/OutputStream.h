#pragma once

#include "String.h"

namespace mye
{

	namespace core
	{

		class OutputStream
		{

		public:

			OutputStream(void);
			~OutputStream(void);

			virtual OutputStream& operator<< (const String &file) = 0;

			virtual OutputStream& operator<< (int x) = 0;
			virtual OutputStream& operator<< (unsigned int x) = 0;
			virtual OutputStream& operator<< (float x) = 0;
			virtual OutputStream& operator<< (double x) = 0;

			virtual size_t Write(const void *data, size_t size) = 0;

		};

	}

}

