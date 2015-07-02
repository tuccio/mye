#pragma once

#include <mye/math/Math.h>
#include <vector>

#include "AlignedAllocator.h"
#include "Singleton.h"
#include "Time.h"
#include "VirtualKeys.h"
#include "EventManager.h"

namespace mye
{

	namespace core
	{

		struct MousePressedKey
		{
			MouseVK key;
			StopWatch  timer;
		};

		class Mouse :
			public Singleton<Mouse>
		{

		public:

			Mouse(void);
			~Mouse(void);

			inline void Press(MouseVK key);
			inline void Release(MouseVK key);

			inline bool IsPressed(MouseVK key) const;

			inline void Move(const mye::math::Vector2 & position);
			inline mye::math::Vector2 GetPosition(void) const;

			inline mye::math::Vector2 GetDelta(void) const;
			inline void ResetDelta(void);

			inline int  GetWheelDelta(void) const;
			inline void SetWheelDelta(int wheelDelta);

			void NotifyHeldKeys(void);

		protected:

			std::vector<MousePressedKey> m_pressedKeys;

		private:

			bool m_keys[MYE_VK_MOUSE_COUNT];

			mye::math::Vector2 m_position;
			mye::math::Vector2 m_delta;

			int m_wheelDelta;

			bool m_firstUpdate;

		};

		struct MouseEventKeyPress :
			IEvent
		{

			MouseEventKeyPress(MouseVK key) :
				IEvent(EventType::MOUSE_KEY_PRESS),
				key(key) { }

			MouseVK key;

		};

		struct MouseEventKeyRelease :
			IEvent
		{

			MouseEventKeyRelease(MouseVK key, FloatSeconds duration) :
				IEvent(EventType::MOUSE_KEY_RELEASE),
				key(key),
				duration(duration) { }

			MouseVK      key;
			FloatSeconds duration;

		};

		struct MouseEventKeyHold :
			IEvent
		{

			MouseEventKeyHold(MouseVK key, FloatSeconds duration) :
				IEvent(EventType::MOUSE_KEY_HOLD),
				key(key),
				duration(duration) { }

			MouseVK      key;
			FloatSeconds duration;

		};

		struct __MYE_ALIGNED(16) MouseEventMove :
			IEvent
		{

			__MYE_DECLARE_ALIGNED_16

			MouseEventMove(const mye::math::Vector2 & from,
			               const mye::math::Vector2 & to) :
			    IEvent(EventType::MOUSE_MOVE),
			    from(from),
			    to(to) { }

			mye::math::Vector2 from;
			mye::math::Vector2 to;

		};

	}

}

#include "Mouse.inl"