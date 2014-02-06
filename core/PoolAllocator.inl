namespace mye
{

	namespace core
	{

		template <typename T>
		PoolAllocator<T>::PoolAllocator(void)
		{
			assert(sizeof(void*) >= sizeof(void*));
			m_free = nullptr;
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

#define MYE_DECLARE_POOL_ALLOCATOR(__TYPE)\
static mye::core::PoolAllocator<__TYPE> m_myePoolAllocator;\
\
void* operator new (std::size_t size);\
void* operator new (std::size_t size, const std::nothrow_t);\
void operator delete (void *p);

#define MYE_DEFINE_POOL_ALLOCATOR(__TYPE)\
mye::core::PoolAllocator<__TYPE> __TYPE::m_myePoolAllocator;\
\
void* __TYPE::operator new (std::size_t size)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().Allocate();\
}\
void* __TYPE::operator new (std::size_t size, const std::nothrow_t)\
{\
return mye::core::PoolAllocator<__TYPE>::GetSingleton().AllocateNoThrow();\
}\
\
void __TYPE::operator delete (void *p)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().Free(p);\
}

#define MYE_DEFINE_TEMPLATE_POOL_ALLOCATOR(__TEMPLATE, __TYPE)\
__TEMPLATE mye::core::PoolAllocator<__TYPE> __TYPE::m_myePoolAllocator;\
\
__TEMPLATE void* __TYPE::operator new (std::size_t size)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().Allocate();\
}\
__TEMPLATE void* __TYPE::operator new (std::size_t size, const std::nothrow_t)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().AllocateNoThrow();\
}\
\
__TEMPLATE void __TYPE::operator delete (void *p)\
{\
	return mye::core::PoolAllocator<__TYPE>::GetSingleton().Free(p);\
}