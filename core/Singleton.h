#pragma once

#include <cassert>

#ifndef NULL
#define NULL 0x0
#endif

namespace mye
{

	namespace core
	{

		template <typename T>
		class Singleton
		{

		public:

			~Singleton(void);

			static T& GetSingleton(void);
			static T* GetSingletonPointer(void);

		protected:

			Singleton(void);

		private:

			static T *m_singleton;

		};

	}

}

#include "Singleton.inl"