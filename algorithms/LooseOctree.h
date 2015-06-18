#pragma once

#include "Boundings.h"
#include "Common.h"
#include "Morton.h"

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

#include <functional>
#include <list>
#include <unordered_map>

#define __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_MAXDEPTH  8
#define __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_LOOSENESS 0.5f

#define __MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX 0

namespace mye
{
	
	namespace algorithms
	{

		namespace detail
		{

			template <typename Object>
			struct LooseOctreeNode
			{
				MortonCode        key;
				std::list<Object> objects;
				uint8_t           childrenMask;
			};

		}

		/* Linear loose octree with 64 bit Morton code for octant location key (hence max depth is 21) */

		template <typename Object,
			typename BoundingVolume,
			typename BoundingVolumeFunctor = Boundings<Object, BoundingVolume>,
			typename Comparator = std::equal_to<Object>>
		class __MYE_MATH_SSE_ALIGNED(16) LooseOctree
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_ALGORITHMS_INLINE LooseOctree(void);

			__MYE_ALGORITHMS_INLINE LooseOctree(const mye::math::Vector3 & center,
			                                    float halfextent,
			                                    int maxdepth              = __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_MAXDEPTH,
			                                    float looseness           = __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_LOOSENESS,
			                                    BoundingVolumeFunctor bvf = BoundingVolumeFunctor(),
												Comparator comparator     = Comparator());

			__MYE_ALGORITHMS_INLINE LooseOctree(const LooseOctree & octree);
			__MYE_ALGORITHMS_INLINE LooseOctree(LooseOctree && octree);

			__MYE_ALGORITHMS_INLINE ~LooseOctree(void);

			__MYE_ALGORITHMS_INLINE bool Create(const mye::math::Vector3 & center,
			                                    float halfextent,
			                                    int maxdepth              = __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_MAXDEPTH,
			                                    float looseness           = __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_LOOSENESS,
			                                    BoundingVolumeFunctor bvf = BoundingVolumeFunctor(),
												Comparator comparator     = Comparator());

			__MYE_ALGORITHMS_INLINE void Destroy(void);

			

			void Insert(const Object & object);
			void Remove(const Object & object);

		private:

			typedef detail::LooseOctreeNode<Object>        __Node;
			typedef std::unordered_map<MortonCode, __Node> __HashMap;
			
			__MYE_ALGORITHMS_INLINE void __Create(const mye::math::Vector3 & center,
			                                      float halfextent,
			                                      int maxdepth,
			                                      float looseness,
			                                      BoundingVolumeFunctor bvf,
			                                      Comparator c);

			__MYE_ALGORITHMS_INLINE void       __Insert(const Object & object, MortonCode location, const mye::math::AABB & parent);
			__MYE_ALGORITHMS_INLINE MortonCode __FindOctant(const Object & object);

			mye::math::Vector3      m_center;
			float                   m_halfextent;
								   
			float                   m_looseness;
			int                     m_maxdepth;
								   
			__HashMap               m_nodes;

			BoundingVolumeFunctor   m_bvf;
			Comparator              m_comparator;


		};

	}

}

#include "LooseOctree.inl"