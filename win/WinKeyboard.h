#pragma once

#include <mye/core/Keyboard.h>
#include <mye/core/Singleton.h>

#include <Windows.h>

namespace mye
{

	namespace win
	{

		class WinKeyboard :
			public mye::core::Keyboard,
			public mye::core::Singleton<WinKeyboard>
		{

		public:

			WinKeyboard(void);
			~WinKeyboard(void);

			bool Hook(void);
			void Unhook(void);

		private:

			HHOOK m_hHook;

		};

	}

}

