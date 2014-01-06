#pragma once

#include "ThreadConfig.h"

namespace mye
{

	namespace core
	{

		class Lockable
		{

		public:

			Lockable(void);
			~Lockable(void);

			void Lock(void);
			void Unlock(void);

		private:

			MYE_RECURSIVE_MUTEX(_mutex)

		};

	}

}

