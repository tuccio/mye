#pragma once

#include "Component.h"

#include "InputModule.h"

namespace mye
{

	namespace core
	{
		class KeyboardComponent :
			public Component,
			public KeyboardListener
		{

		public:

			KeyboardComponent(void);
			~KeyboardComponent(void);

			KeyboardComponent * Clone(void) const;

			void OnAttach(GameObject * gameObject);
			void OnDetach(void);

			void OnKeyboardKeyPress(KeyboardVK key);
			void OnKeyboardKeyRelease(KeyboardVK key, FloatSeconds time);
			void OnKeyboardKeyHold(KeyboardVK key, FloatSeconds time);

		};

	}

}
