#pragma once

#include <cassert>

namespace mye
{

	namespace core
	{

		template <typename T>
		class Singleton
		{

		public:

			~Singleton(void);

			inline static T& GetSingleton(void);
			inline static T* GetSingletonPointer(void);

		protected:

			Singleton(void);

		private:

			static T *m_singleton;

		};

	}

}

#include "Singleton.inl"