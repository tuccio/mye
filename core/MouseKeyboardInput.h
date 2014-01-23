#pragma once

#include "Keyboard.h"
#include "Mouse.h"

#include "InputModule.h"

namespace mye
{

	namespace core
	{

		class MouseKeyboardInput :
			public InputModule
		{

		public:

			MouseKeyboardInput(void);
			~MouseKeyboardInput(void);

			bool Init(void);
			void ShutDown(void);

			bool HasKeyboard(void) const;
			Keyboard* GetKeyboard(void);
			const Keyboard* GetKeyboard(void) const;

			bool HasMouse(void) const;
			Mouse* GetMouse(void);
			const Mouse* GetMouse(void) const;

		private:

			Keyboard m_keyboard;
			Mouse m_mouse;

		};

	}

}
