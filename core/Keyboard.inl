namespace mye
{

	namespace core
	{

		void Keyboard::Press(KeyboardVK key)
		{
			m_keys[key].Start();
		}

		Milliseconds Keyboard::PressTime(KeyboardVK key) const
		{
			return m_keys[key].GetElapsedTime();
		}

		Milliseconds Keyboard::Release(KeyboardVK key)
		{
			return m_keys[key].Stop();
		}

		bool Keyboard::IsPressed(KeyboardVK key) const
		{
			return m_keys->IsRunning();
		}

	}

}