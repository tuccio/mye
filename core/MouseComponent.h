#pragma once

#include "Component.h"

#include "InputModule.h"
#include "EventManager.h"

namespace mye
{

	namespace core
	{
		class MouseComponent :
			public Component,
			public IEventListener
		{

		public:

			MouseComponent(void);
			~MouseComponent(void);

			MouseComponent * Clone(void) const;

			void OnAttach(GameObject * gameObject);
			void OnDetach(void);

			void OnEvent(const IEvent * e);

		};

	}

}
