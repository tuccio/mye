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
	return m_myePoolAllocator.Allocate();\
}\
void* __TYPE::operator new (std::size_t size, const std::nothrow_t)\
{\
return m_myePoolAllocator.AllocateNoThrow();\
}\
\
void __TYPE::operator delete (void *p)\
{\
	return m_myePoolAllocator.Free(p);\
}

#define MYE_DEFINE_TEMPLATE_POOL_ALLOCATOR(__TYPE, __T1, __T2)\
\
template <__T1, __T2> mye::core::PoolAllocator<__TYPE<__T1, __T2>> __TYPE<__T1, __T2>::m_myePoolAllocator;\
\
template <__T1, __T2> void* __TYPE<__T1, __T2>::operator new (std::size_t size)\
{\
	return m_myePoolAllocator.Allocate();\
}\
template <__T1, __T2> void* __TYPE<__T1, __T2>::operator new (std::size_t size, const std::nothrow_t)\
{\
	return m_myePoolAllocator.AllocateNoThrow();\
}\
\
template <__T1, __T2> void __TYPE<__T1, __T2>::operator delete (void *p)\
{\
	return m_myePoolAllocator.Free(p);\
}

/*
#define MYE_DEFINE_TEMPLATE_POOL_ALLOCATOR(__TEMPLATE, __TYPE)\
__TEMPLATE mye::core::PoolAllocator<__TYPE> __TYPE::m_myePoolAllocator;\
\
__TEMPLATE void* __TYPE::operator new (std::size_t size)\
{\
	return m_myePoolAllocator.Allocate();\
}\
	__TEMPLATE void* __TYPE::operator new (std::size_t size, const std::nothrow_t)\
{\
	return m_myePoolAllocator.AllocateNoThrow();\
}\
\
__TEMPLATE void __TYPE::operator delete (void *p)\
{\
	return m_myePoolAllocator.Free(p);\
}*/