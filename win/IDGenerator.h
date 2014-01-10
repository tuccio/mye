#pragma once

#include <mye/core/Singleton.h>
#include <Windows.h>

namespace mye
{

	namespace win
	{

		class IDGenerator :
			public mye::core::Singleton<IDGenerator>
		{

		public:

			typedef DWORD ID;

			IDGenerator(void);
			~IDGenerator(void);

			ID Generate(void);
			ID Generate(int n);

		private:

			ID m_nextID;

		};

	}
}
