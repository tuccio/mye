
namespace mye
{

	namespace algorithms
	{

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE LooseOctree<O, BV, BVF, C>::LooseOctree(void) :
			m_nodes(nullptr)
		{

		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE LooseOctree<O, BV, BVF, C>::LooseOctree(const mye::math::Vector3 & center,
		                                                                float halfextent,
		                                                                int maxdepth,
		                                                                float looseness,
		                                                                BVF bvf,
																		C comparator)
		{
			__Create(center, halfextent, maxdepth, looseness, bvf, comparator);
		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE LooseOctree<O, BV, BVF, C>::LooseOctree(const LooseOctree & octree)
		{

			__Create(octree.m_center, octree.m_halfextent, octree.m_maxdepth, octree.m_looseness, octree.m_bvf);

			m_nodes = octree.m_nodes;

		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE LooseOctree<O, BV, BVF, C>::LooseOctree(LooseOctree && octree) :
			m_center(octree.m_center),
			m_looseness(octree.m_looseness),
			m_maxdepth(octree.m_maxdepth),
			m_bvf(octree.m_bvf),
			m_comparator(octree.m_comparator),
			m_nodes(octree.m_nodes)
		{
			octree.m_nodes = nullptr;
		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE LooseOctree<O, BV, BVF, C>::~LooseOctree(void)
		{
			Destroy();
		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE void LooseOctree<O, BV, BVF, C>::__Create(const mye::math::Vector3 & center,
		                                                                  float halfextent,
		                                                                  int maxdepth,
		                                                                  float looseness,
		                                                                  BVF bvf,
		                                                                  C comparator)
		{

			m_center     = center;
			m_halfextent = halfextent;
			m_looseness  = looseness;
			m_maxdepth   = maxdepth;
			m_bvf        = bvf;
			m_comparator = comparator;

			__Node root;

			root.childrenMask = 0;
			root.key          = 0;

			m_nodes.insert(std::make_pair(__MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX, root));

		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE bool LooseOctree<O, BV, BVF, C>::Create(const mye::math::Vector3 & center,
		                                                                float halfextent,
		                                                                int maxdepth,
		                                                                float looseness,
		                                                                BVF bvf,
		                                                                C comparator)
		{

			if (m_nodes)
			{
				return false;
			}

			__Create(center, halfextent, maxdepth, looseness, bvf, comparator);

			return true;

		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE void LooseOctree<O, BV, BVF, C>::Destroy(void)
		{
			m_nodes.clear();
		}

		template <typename O, typename BV, typename BVF, typename C>
		__MYE_ALGORITHMS_INLINE void LooseOctree<O, BV, BVF, C>::Insert(const O & object)
		{

			int index = 0;
			AABB aabb = AABB::FromCenterHalfExtents(m_center, m_halfextent);

			//__Insert(object, index, aabb);			

		}


	}

}