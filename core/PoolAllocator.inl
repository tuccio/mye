namespace mye
{

	namespace core
	{

		template <typename T, typename Allocator>
		PoolAllocator<T, Allocator>::PoolAllocator(void)
		{
			assert(sizeof(void*) >= sizeof(void*));
			m_free = nullptr;
		}

		template <typename T, typename Allocator>
		void* PoolAllocator<T, Allocator>::Allocate(size_t size)
		{

			if (m_free)
			{
				void *firstFree = m_free;
				memcpy(&m_free, m_free, size);
				return firstFree;
			}
			else
			{
				
				return Allocator::Allocate(size);
			}

		}

		template <typename T, typename Allocator>
		void* PoolAllocator<T, Allocator>::AllocateNoThrow(size_t size)
		{

			if (m_free)
			{
				void *firstFree = m_free;
				memcpy(&m_free, m_free, size);
				return firstFree;
			}
			else
			{
				return Allocator::AllocateNoThrow(size);
			}

		}

		template <typename T, typename Allocator>
		void PoolAllocator<T, Allocator>::Free(void *p)
		{
			memcpy(p, &m_free, sizeof(void*));
			m_free = p;
		}

	}

}

#define MYE_DECLARE_POOL_ALLOCATOR_IMPL(__TYPE, __ALLOCATOR)\
	__MYE_USE_ALLOCATOR(mye::core::PoolAllocator<__TYPE BOOST_PP_COMMA() __ALLOCATOR>)

#define MYE_DEFINE_POOL_ALLOCATOR_IMPL(__TYPE, __ALLOCATOR)\
void* mye::core::PoolAllocator<__TYPE, __ALLOCATOR>::m_free;

#define MYE_DECLARE_POOL_ALLOCATOR(__TYPE) MYE_DECLARE_POOL_ALLOCATOR_IMPL(__TYPE, mye::core::DefaultAllocator)
#define MYE_DEFINE_POOL_ALLOCATOR(__TYPE) MYE_DEFINE_POOL_ALLOCATOR_IMPL(__TYPE, mye::core::DefaultAllocator)

#define MYE_DECLARE_POOL_ALLOCATOR_ALIGNED(__TYPE, __ALIGNMENT) MYE_DECLARE_POOL_ALLOCATOR_IMPL(__TYPE, mye::core::AlignedAllocator<__ALIGNMENT>)
#define MYE_DEFINE_POOL_ALLOCATOR_ALIGNED(__TYPE, __ALIGNMENT) MYE_DEFINE_POOL_ALLOCATOR_IMPL(__TYPE, mye::core::AlignedAllocator<__ALIGNMENT>)

#define MYE_DECLARE_POOL_ALLOCATOR_ALIGNED_16 MYE_DECLARE_POOL_ALLOCATOR_ALIGNED(__TYPE, 16)
#define MYE_DEFINE_POOL_ALLOCATOR_ALIGNED_16 MYE_DEFINE_POOL_ALLOCATOR_ALIGNED(__TYPE, 16)

#define MYE_DECLARE_POOL_ALLOCATOR_ALIGNED_32 MYE_DECLARE_POOL_ALLOCATOR_ALIGNED(__TYPE, 32)
#define MYE_DEFINE_POOL_ALLOCATOR_ALIGNED_32 MYE_DEFINE_POOL_ALLOCATOR_ALIGNED(__TYPE, 32)

#define MYE_DECLARE_POOL_ALLOCATOR_ALIGNED_64 MYE_DECLARE_POOL_ALLOCATOR_ALIGNED(__TYPE, 64)
#define MYE_DEFINE_POOL_ALLOCATOR_ALIGNED_64 MYE_DEFINE_POOL_ALLOCATOR_ALIGNED(__TYPE, 64)

#define MYE_DECLARE_POOL_ALLOCATOR_ALIGNED_128 MYE_DECLARE_POOL_ALLOCATOR_ALIGNED(__TYPE, 128)
#define MYE_DEFINE_POOL_ALLOCATOR_ALIGNED_128 MYE_DEFINE_POOL_ALLOCATOR_ALIGNED(__TYPE, 128)