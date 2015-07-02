#pragma once

#include <mye/math/Math.h>
#include <mye/core/Mouse.h>
#include <mye/core/VirtualKeys.h>

#include "Windows.h"

namespace mye
{

	namespace win
	{

		class WinMouse :
			public mye::core::Mouse
		{

		public:

			WinMouse(void);
			~WinMouse(void);

			bool Hook(void);
			void Unhook(void);

		private:

			HHOOK m_hHook;

		};

	}

}