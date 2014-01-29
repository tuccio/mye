#pragma once

#include "VirtualKeys.h"
#include <vector>

namespace mye
{

	namespace core
	{

		class Keyboard
		{

		public:

			class KeyboardListener;

			Keyboard(void);
			~Keyboard(void);

			inline void Press(KeyboardVK key);
			inline void Release(KeyboardVK key);

			inline bool IsPressed(KeyboardVK key) const;

			bool Hook(void);

			void AddListener(KeyboardListener *listener);
			void RemoveListener(KeyboardListener *listener);

		protected:

			std::vector<KeyboardListener*> m_listeners;

		private:

			bool m_keys[MYE_VK_COUNT];


		};

		class KeyboardListener
		{

		public:

			virtual void OnKeyPress(KeyboardVK key);
			virtual void OnKeyRelease(KeyboardVK key);

		};

	}

}

#include "Keyboard.inl"