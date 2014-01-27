namespace mye
{

	namespace core
	{

		void Mouse::Press(MouseVK key)
		{
			m_keys[key] = true;
		}

		void Mouse::Release(MouseVK key)
		{
			m_keys[key] = false;
		}

		bool Mouse::IsPressed(MouseVK key) const
		{
			return m_keys[key];
		}

		void Mouse::Move(const mye::math::Vector2f &position)
		{
			m_delta = position - m_position;
			m_position = position;
		}

		mye::math::Vector2f Mouse::GetPosition(void) const
		{
			return m_position;
		}

		mye::math::Vector2f Mouse::GetDelta(void) const
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