#pragma once

#include <mye/math/Math.h>

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
			inline void Release(MouseVK key);

			inline bool IsPressed(MouseVK key) const;

			inline void Move(const mye::math::Vector2f &position);
			inline mye::math::Vector2f GetPosition(void) const;

			inline mye::math::Vector2f GetDelta(void) const;
			inline void ResetDelta(void);

			inline int GetWheelDelta(void) const;
			inline void SetWheelDelta(int wheelDelta);

		private:

			bool m_keys[MYE_VK_MOUSE_COUNT];

			mye::math::Vector2f m_position;
			mye::math::Vector2f m_delta;

			int m_wheelDelta;

		};

	}

}

#include "Mouse.inl"