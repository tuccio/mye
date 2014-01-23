#pragma once

#include "VirtualKeys.h"
#include "Time.h"

namespace mye
{

	namespace core
	{

		class Keyboard
		{

		public:

			Keyboard(void);
			~Keyboard(void);

			inline void Press(KeyboardVK key);
			inline Milliseconds PressTime(KeyboardVK key) const;
			inline Milliseconds Release(KeyboardVK key);

			inline bool IsPressed(KeyboardVK key) const;

		private:

			StopWatch m_keys[MYE_VK_COUNT];

		};

	}

}

#include "Keyboard.inl"