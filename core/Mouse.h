#pragma once

#include <mye/math/Math.h>

#include "Time.h"
#include "VirtualKeys.h"

namespace mye
{

	namespace core
	{

		class Mouse
		{

		public:

			Mouse(void);
			~Mouse(void);

			inline void Press(MouseVK key);
			inline Milliseconds PressTime(MouseVK key) const;
			inline Milliseconds Release(MouseVK key);

			inline bool IsPressed(MouseVK key) const;

			inline void Move(const mye::math::Vector2f &position);

			inline mye::math::Vector2f GetPosition(void) const;
			inline mye::math::Vector2f GetDelta(void) const;

		private:

			StopWatch m_keys[MYE_VK_MOUSE_COUNT];

			mye::math::Vector2f m_position;
			mye::math::Vector2f m_delta;

		};

	}

}

#include "Mouse.inl"