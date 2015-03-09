#pragma once

#include <mye/math/Math.h>
#include <vector>

#include "Time.h"
#include "VirtualKeys.h"

namespace mye
{

	namespace core
	{

		class MouseListener;

		struct MousePressedKey
		{
			MouseVK key;
			StopWatch  timer;
		};

		class Mouse
		{

		public:

			Mouse(void);
			~Mouse(void);

			inline void Press(MouseVK key);
			inline void Release(MouseVK key);

			inline bool IsPressed(MouseVK key) const;

			inline void Move(const mye::math::Vector2 &position);
			inline mye::math::Vector2 GetPosition(void) const;

			inline mye::math::Vector2 GetDelta(void) const;
			inline void ResetDelta(void);

			inline int GetWheelDelta(void) const;
			inline void SetWheelDelta(int wheelDelta);

			void AddListener(MouseListener * listener);
			void RemoveListener(MouseListener * listener);

			void NotifyHeldKeys(void);

		protected:

			std::vector<MouseListener*> m_listeners;
			std::vector<MousePressedKey>        m_pressedKeys;

		private:

			bool m_keys[MYE_VK_MOUSE_COUNT];

			mye::math::Vector2 m_position;
			mye::math::Vector2 m_delta;

			int m_wheelDelta;

			bool m_firstUpdate;

		};

		class MouseListener
		{

		public:

			virtual void OnMouseKeyPress(MouseVK key) { }
			virtual void OnMouseKeyRelease(MouseVK key, FloatSeconds time) { }
			virtual void OnMouseKeyHold(MouseVK key, FloatSeconds time) { }

			virtual void OnMouseMove(const mye::math::Vector2 & from,
									 const mye::math::Vector2 & to) { }

		};

	}

}

#include "Mouse.inl"