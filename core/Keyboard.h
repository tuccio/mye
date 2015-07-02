#pragma once

#include "EventManager.h"
#include "Singleton.h"
#include "Time.h"
#include "VirtualKeys.h"

#include <vector>

namespace mye
{

	namespace core
	{

		struct KeyboardPressedKey
		{
			KeyboardVK key;
			StopWatch  timer;
		};

		class Keyboard :
			public Singleton<Keyboard>
		{

		public:

			Keyboard(void);
			~Keyboard(void);

			inline void Press(KeyboardVK key);
			inline void Release(KeyboardVK key);

			inline bool IsPressed(KeyboardVK key) const;

			bool Hook(void);

			void NotifyHeldKeys(void);

		protected:

			std::vector<KeyboardPressedKey> m_pressedKeys;

		private:

			bool m_keys[MYE_VK_COUNT];


		};

		struct KeyboardEventKeyPress :
			IEvent
		{

			KeyboardEventKeyPress(KeyboardVK key) :
				IEvent(EventType::KEYBOARD_KEY_PRESS),
				key(key) { }

			KeyboardVK key;

		};

		struct KeyboardEventKeyRelease :
			IEvent
		{

			KeyboardEventKeyRelease(KeyboardVK key, FloatSeconds duration) :
				IEvent(EventType::KEYBOARD_KEY_RELEASE),
				key(key) { }

			KeyboardVK      key;
			FloatSeconds duration;

		};

		struct KeyboardEventKeyHold :
			IEvent
		{

			KeyboardEventKeyHold(KeyboardVK key, FloatSeconds duration) :
				IEvent(EventType::KEYBOARD_KEY_HOLD),
				key(key),
				duration(duration){ }

			KeyboardVK      key;
			FloatSeconds duration;

		};

	}	

}

#include "Keyboard.inl"