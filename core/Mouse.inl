#include <algorithm>

namespace mye
{

	namespace core
	{

		void Mouse::Press(MouseVK key)
		{

			if (!m_keys[key])
			{

				m_keys[key] = true;

				MYE_EVENT_MANAGER_ENQUEUE(MouseEventKeyPress, key);

				MousePressedKey pk;
				pk.key = key;
				pk.timer.Start();

				m_pressedKeys.push_back(pk);

			}

		}

		void Mouse::Release(MouseVK key)
		{

			m_keys[key] = false;

			auto it = std::find_if(m_pressedKeys.begin(),
								   m_pressedKeys.end(),
								   [key] (const MousePressedKey & pk) -> bool { return pk.key == key; });

			if (it != m_pressedKeys.end())
			{

				FloatSeconds time = it->timer.GetElapsedTime() / 1000.0f;

				MYE_EVENT_MANAGER_ENQUEUE(MouseEventKeyRelease, key, time);

				m_pressedKeys.erase(it);

			}

		}

		bool Mouse::IsPressed(MouseVK key) const
		{
			return m_keys[key];
		}

		void Mouse::Move(const mye::math::Vector2 & position)
		{

			if (!m_firstUpdate)
			{

				mye::math::Vector2 oldPosition = m_position;
				m_delta = position - m_position;
				m_position = position;

				MYE_EVENT_MANAGER_ENQUEUE(MouseEventMove, oldPosition, m_position);
				
			}
			else
			{

				m_firstUpdate = false;
				m_position = position;

			}
			

		}

		mye::math::Vector2 Mouse::GetPosition(void) const
		{
			return m_position;
		}

		mye::math::Vector2 Mouse::GetDelta(void) const
		{
			return m_delta;
		}

		void Mouse::ResetDelta(void)
		{
			m_delta.x() = 0;
			m_delta.y() = 0;
		}

		int Mouse::GetWheelDelta(void) const
		{
			return m_wheelDelta;
		}

		void Mouse::SetWheelDelta(int wheelDelta)
		{
			m_wheelDelta = wheelDelta;
		}

	}

}