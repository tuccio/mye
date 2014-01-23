namespace mye
{

	namespace core
	{

		void Mouse::Press(MouseVK key)
		{
			m_keys[key].Start();
		}

		Milliseconds Mouse::PressTime(MouseVK key) const
		{
			return m_keys[key].GetElapsedTime();
		}

		Milliseconds Mouse::Release(MouseVK key)
		{
			return m_keys[key].Stop();
		}

		bool Mouse::IsPressed(MouseVK key) const
		{
			m_keys->IsRunning();
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

	}

}