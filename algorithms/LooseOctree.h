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

namespace mye
{
	
	namespace algorithms
	{

		enum LooseOctreeChildren
		{
			__MYE_ALGORITHMS_LOOSEOCTREE_FRONT_BOTTOM_LEFT  = 0, // 000
			__MYE_ALGORITHMS_LOOSEOCTREE_FRONT_BOTTOM_RIGHT = 1, // 001
			__MYE_ALGORITHMS_LOOSEOCTREE_FRONT_TOP_LEFT     = 2, // 010
			__MYE_ALGORITHMS_LOOSEOCTREE_FRONT_TOP_RIGHT    = 3, // 011
			__MYE_ALGORITHMS_LOOSEOCTREE_BACK_BOTTOM_LEFT   = 4, // 100
			__MYE_ALGORITHMS_LOOSEOCTREE_BACK_BOTTOM_RIGHT  = 5, // 101
			__MYE_ALGORITHMS_LOOSEOCTREE_BACK_TOP_LEFT      = 6, // 110
			__MYE_ALGORITHMS_LOOSEOCTREE_BACK_TOP_RIGHT     = 7, // 111
		};

		namespace detail
		{

			template <typename Object>
			struct LooseOctreeNode
			{
				
				std::list<Object> objects;
				uint8_t           childrenMask;
				uint32_t          occupancy;

				__MYE_ALGORITHMS_INLINE LooseOctreeNode(void) :
					childrenMask(0),
					occupancy(0) { }

			};

		}


		/* 
		 *
		 * Loose octree with looseness factor k = 2, represented as a linear octree
		 * with 64 bit Morton code for octant location key (hence max depth is 21)
		 *
		 * Insertion and removal are O(1)
		 *
		 */

		template <typename Object,
		          typename BoundingVolume = mye::math::AABB,
		          typename BoundingVolumeFunctor = Boundings<Object, BoundingVolume>,
		          typename Comparator  = std::equal_to<Object>>
		class __MYE_MATH_SSE_ALIGNED(16) LooseOctree
		{

		public:

			__MYE_MATH_SSE_ALIGNED_ALLOCATOR(16)

			__MYE_ALGORITHMS_INLINE LooseOctree(void);

			__MYE_ALGORITHMS_INLINE LooseOctree(const mye::math::Vector3 & center,
			                                    float halfextent,
			                                    unsigned int maxdepth         = __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_MAXDEPTH,
												BoundingVolumeFunctor functor = BoundingVolumeFunctor(),
												Comparator comparator         = Comparator());

			__MYE_ALGORITHMS_INLINE LooseOctree(const LooseOctree & octree);
			__MYE_ALGORITHMS_INLINE LooseOctree(LooseOctree && octree);

			__MYE_ALGORITHMS_INLINE ~LooseOctree(void);

			__MYE_ALGORITHMS_INLINE bool Create(const mye::math::Vector3 & center,
			                                    float halfextent,
			                                    unsigned int maxdepth         = __MYE_ALGORITHMS_LOOSEOCTREE_DEFAULT_MAXDEPTH,
												BoundingVolumeFunctor functor = BoundingVolumeFunctor(),
												Comparator comparator         = Comparator());

			__MYE_ALGORITHMS_INLINE void Destroy(void);


			__MYE_ALGORITHMS_INLINE bool Insert(const Object & object);
			__MYE_ALGORITHMS_INLINE bool Insert(const Object & object, const BoundingVolume & volume);

			__MYE_ALGORITHMS_INLINE bool Remove(const Object & object);
			__MYE_ALGORITHMS_INLINE bool Remove(const Object & object, const BoundingVolume & volume);

			template <typename QueryType, typename Visitor>
			__MYE_ALGORITHMS_INLINE void Query(const QueryType & query, Visitor visitor);

			__MYE_ALGORITHMS_INLINE bool Pick(const mye::math::Ray & ray, Object & result, mye::math::Real & t);

			template <typename Visitor>
			__MYE_ALGORITHMS_INLINE void TraverseAABBs(Visitor visitor);

			__MYE_ALGORITHMS_INLINE mye::math::AABB GetAABB(void) const;

			__MYE_ALGORITHMS_INLINE size_t GetSize(void) const;

			__MYE_ALGORITHMS_INLINE void Shrink(void);

		private:

			typedef detail::LooseOctreeNode<Object> __Node;

			typedef std::unordered_map<MortonCode, __Node> __NodeHashMap;
			typedef typename __NodeHashMap::iterator       __NodeHashMapIterator;

			mye::math::Vector3    m_center;
			float                 m_halfextent;

			unsigned int          m_maxdepth;

			BoundingVolumeFunctor m_functor;
			Comparator            m_comparator;

			__NodeHashMap         m_nodes;

			/* Private methods */

			__MYE_ALGORITHMS_INLINE void __Create(const mye::math::Vector3 & center,
												  float halfextent,
												  unsigned int maxdepth,
												  BoundingVolumeFunctor functor,
												  Comparator c);

			__MYE_ALGORITHMS_INLINE typename __NodeHashMapIterator __CreateOctant(MortonCode octantLocation);

			__MYE_ALGORITHMS_INLINE MortonCode   __FindFittingOctant(const mye::math::AABB & aabb) const;

			__MYE_ALGORITHMS_INLINE unsigned int        __GetOctantDepth(MortonCode octant) const;
			__MYE_ALGORITHMS_INLINE mye::math::AABB     __GetOctantAABB(MortonCode octant) const;
			__MYE_ALGORITHMS_INLINE mye::math::Vector3u __GetOctantCoordinates(MortonCode octant) const;

			template <typename QueryType, typename Visitor>
			__MYE_ALGORITHMS_INLINE void __Traverse(MortonCode currentLocation, 
													const mye::math::AABB & current,
													const QueryType & query,
													Visitor visitor);

			__MYE_ALGORITHMS_INLINE void __Pick(MortonCode currentLocation,
												const mye::math::AABB & current,
												const mye::math::Ray  & ray,
												mye::math::Real & minDistance,
												Object * & object);


			template <typename Visitor>
			__MYE_ALGORITHMS_INLINE void __DFS(MortonCode octant, Visitor visitor);

			__MYE_ALGORITHMS_INLINE void __IncreaseOccupancy(__NodeHashMapIterator octant);
			__MYE_ALGORITHMS_INLINE void __DecreaseOccupancy(__NodeHashMapIterator octant);


		};

	}

}

#include "LooseOctree.inl"