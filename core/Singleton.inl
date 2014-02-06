namespace mye
{

	namespace core
	{

		template <typename T>
		T* Singleton<T>::m_singleton = nullptr;

		template <typename T>
		Singleton<T>::Singleton(void)
		{
			assert(!m_singleton);
			m_singleton = static_cast<T*>(this);
		}

		template <typename T>
		Singleton<T>::~Singleton(void)
		{
			m_singleton = nullptr;
		}

		template <typename T>
		T& Singleton<T>::GetSingleton(void)
		{
			assert(m_singleton);
			return *m_singleton;
		}

		template <typename T>
		T* Singleton<T>::GetSingletonPointer(void)
		{
			return m_singleton;
		}

	}
}
