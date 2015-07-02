#include <algorithm>

namespace mye
{

	namespace core
	{

		void Keyboard::Press(KeyboardVK key)
		{

			if (!m_keys[key])
			{

				m_keys[key] = true;

				MYE_EVENT_MANAGER_ENQUEUE(KeyboardEventKeyPress, key);

				KeyboardPressedKey pk;
				pk.key = key;
				pk.timer.Start();

				m_pressedKeys.push_back(pk);

			}

		}

		void Keyboard::Release(KeyboardVK key)
		{

			m_keys[key] = false;

			auto it = std::find_if(m_pressedKeys.begin(),
								   m_pressedKeys.end(),
								   [key] (const KeyboardPressedKey & pk) -> bool { return pk.key == key; });

			if (it != m_pressedKeys.end())
			{

				FloatSeconds time = it->timer.GetElapsedTime() / 1000.0f;

				MYE_EVENT_MANAGER_ENQUEUE(KeyboardEventKeyRelease, key, time);

				m_pressedKeys.erase(it);
			}
			

		}

		bool Keyboard::IsPressed(KeyboardVK key) const
		{
			return m_keys[key];
		}

	}

}