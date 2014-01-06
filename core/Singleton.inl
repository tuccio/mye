namespace mye
{

	namespace core
	{

		template <typename T>
		T* Singleton<T>::_singleton = NULL;

		template <typename T>
		Singleton<T>::Singleton(void)
		{
			assert(!_singleton);
			_singleton = static_cast<T*>(this);
		}

		template <typename T>
		Singleton<T>::~Singleton(void)
		{
			_singleton = NULL;
		}

		template <typename T>
		T& Singleton<T>::GetSingleton(void)
		{
			assert(_singleton);
			return *_singleton;
		}

		template <typename T>
		T* Singleton<T>::GetSingletonPointer(void)
		{
			return _singleton;
		}

	}
}
