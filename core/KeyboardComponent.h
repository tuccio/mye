#pragma once

#include "Component.h"

#include "InputModule.h"
#include "EventManager.h"

namespace mye
{

	namespace core
	{
		class KeyboardComponent :
			public Component,
			public IEventListener
		{

		public:

			KeyboardComponent(void);
			~KeyboardComponent(void);

			KeyboardComponent * Clone(void) const;

			void OnAttach(GameObject * gameObject);
			void OnDetach(void);

			void OnEvent(const IEvent * e);

		};

	}

}
