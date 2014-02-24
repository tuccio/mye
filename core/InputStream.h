#pragma once

#include "String.h"

namespace mye
{

	namespace core
	{

		class InputStream
		{

		public:

			InputStream(void);
			~InputStream(void);

			virtual InputStream& operator>> (String &s) = 0;
			virtual size_t Read(void *data, size_t size) = 0;

		};

	}

}

