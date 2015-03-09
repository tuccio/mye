#pragma once

#include "Time.h"
#include "VirtualKeys.h"
#include <vector>

namespace mye
{

	namespace core
	{

		class KeyboardListener;

		struct KeyboardPressedKey
		{
			KeyboardVK key;
			StopWatch  timer;
		};

		class Keyboard
		{

		public:

			Keyboard(void);
			~Keyboard(void);

			inline void Press(KeyboardVK key);
			inline void Release(KeyboardVK key);

			inline bool IsPressed(KeyboardVK key) const;

			bool Hook(void);

			void AddListener(KeyboardListener * listener);
			void RemoveListener(KeyboardListener * listener);

			void NotifyHeldKeys(void);

		protected:

			std::vector<KeyboardListener*> m_listeners;
			std::vector<KeyboardPressedKey>        m_pressedKeys;

		private:

			bool m_keys[MYE_VK_COUNT];


		};

		class KeyboardListener
		{

		public:

			virtual void OnKeyboardKeyPress(KeyboardVK key);
			virtual void OnKeyboardKeyRelease(KeyboardVK key, FloatSeconds time);
			virtual void OnKeyboardKeyHold(KeyboardVK key, FloatSeconds time);

		};

	}	

}

#include "Keyboard.inl"