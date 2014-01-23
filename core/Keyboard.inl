namespace mye
{

	namespace core
	{

		void Keyboard::Press(KeyboardVK key)
		{
			m_keys[key] = true;
		}

		void Keyboard::Release(KeyboardVK key)
		{
			m_keys[key] = false;
		}

		bool Keyboard::IsPressed(KeyboardVK key) const
		{
			return m_keys[key];
		}

	}

}