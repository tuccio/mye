#pragma once

#include "WinKeyboard.h"
#include "WinMouse.h"

#include <mye/core/InputModule.h>

namespace mye
{

	namespace win
	{

		class MouseKeyboardInput :
			public mye::core::InputModule
		{

		public:

			MouseKeyboardInput(void);
			~MouseKeyboardInput(void);

			bool Init(void);
			void ShutDown(void);

			bool HasKeyboard(void) const;
			WinKeyboard* GetKeyboard(void);
			const WinKeyboard* GetKeyboard(void) const;

			bool HasMouse(void) const;
			WinMouse* GetMouse(void);
			const mye::core::Mouse* GetMouse(void) const;

			void Preupdate(void);

		private:

			WinKeyboard m_keyboard;
			WinMouse m_mouse;

		};

	}

}
