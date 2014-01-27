#pragma once

#include "VirtualKeys.h"

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
			inline void Release(KeyboardVK key);

			inline bool IsPressed(KeyboardVK key) const;

			bool Hook(void);

		private:

			bool m_keys[MYE_VK_COUNT];

		};

	}

}

#include "Keyboard.inl"