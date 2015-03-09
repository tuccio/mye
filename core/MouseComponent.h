#pragma once

#include "Component.h"

#include "InputModule.h"

namespace mye
{

	namespace core
	{
		class MouseComponent :
			public Component,
			public MouseListener
		{

		public:

			MouseComponent(void);
			~MouseComponent(void);

			MouseComponent * Clone(void) const;

			void OnAttach(GameObject * gameObject);
			void OnDetach(void);

			void OnMouseKeyPress(MouseVK key);
			void OnMouseKeyRelease(MouseVK key, FloatSeconds time);
			void OnMouseKeyHold(MouseVK key, FloatSeconds time);

			void OnMouseMove(const mye::math::Vector2 & from,
							 const mye::math::Vector2 & to);

		};

	}

}
