namespace mye
{

	namespace core
	{

		template <typename T>
		PoolAllocator<T>::PoolAllocator(void)
		{
			assert(sizeof(void*) >= sizeof(void*));
			m_free = NULL;
		}

		template <typename T>
		void* PoolAllocator<T>::Allocate(void)
		{

			if (m_free)
			{
				void *firstFree = m_free;
				memcpy(&m_free, m_free, sizeof(void*));
				return firstFree;
			}
			else
			{
				void *p = malloc(sizeof(T));
				if (!p)
				{
					throw std::bad_alloc();
				}
				return p;
			}

		}

		template <typename T>
		void* PoolAllocator<T>::AllocateNoThrow(void)
		{

			if (m_free)
			{
				void *firstFree = m_free;
				memcpy(&m_free, m_free, sizeof(void*));
				return firstFree;
			}
			else
			{
				return malloc(sizeof(T));
			}

		}

		template <typename T>
		void PoolAllocator<T>::Free(void *p)
		{
			memcpy(p, &m_free, sizeof(void*));
			m_free = p;
		}

	}

}

#define MYE_USE_POOL_ALLOCATOR(__TYPE)\
void* operator new (std::size_t size)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().Allocate();\
}\
void* operator new (std::size_t size, const std::nothrow_t)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().AllocateNoThrow();\
}\
\
void operator delete (void *p)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().Free(p);\
}