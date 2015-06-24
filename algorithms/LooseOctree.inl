

#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/list/for_each.hpp>

#include <deque>

#define __MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION template <typename Object, typename BoundingVolume, typename BoundingVolumeFunctor, typename Comparator>
#define __MYE_ALGORITHMS_LOOSEOCTREE_TYPE                 LooseOctree<Object, BoundingVolume, BoundingVolumeFunctor, Comparator>

#define __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILD_MASK(Child)                 (1 << (Child))
#define __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILDREN_MASK_LOOP(r, data, elem) | __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILD_MASK(elem)
#define __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILDREN_MASK(...)                (0 BOOST_PP_LIST_FOR_EACH(__MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILDREN_MASK_LOOP, 0, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)))

#define __MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX (1)

namespace mye
{

	namespace algorithms
	{

		

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::LooseOctree(void)
		{

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::LooseOctree(const mye::math::Vector3 & center,
		                                                                       float halfextent,
																			   unsigned int maxdepth,
																			   BoundingVolumeFunctor functor,
																			   Comparator comparator)
		{
			__Create(center, halfextent, maxdepth, m_functor, comparator);
		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::LooseOctree(const LooseOctree & octree)
		{

			__Create(octree.m_center, octree.m_halfextent, octree.m_maxdepth, octree.m_functor, octree.m_comparator);

			m_nodes = octree.m_nodes;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::LooseOctree(LooseOctree && octree) :
			m_center    (std::move(octree.m_center)),
			m_maxdepth  (std::move(octree.m_maxdepth)),
			m_bvf       (std::move(octree.m_bvf)),
			m_comparator(std::move(octree.m_comparator)),
			m_nodes     (std::move(octree.m_nodes))
		{
			octree.Destroy();
		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::~LooseOctree(void)
		{
			Destroy();
		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE bool __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::Create(const mye::math::Vector3 & center,
		                                                                       float halfextent,
																			   unsigned int maxdepth,
																		       BoundingVolumeFunctor functor,
		                                                                       Comparator comparator)
		{

			if (!m_nodes.empty())
			{
				return false;
			}

			__Create(center, halfextent, maxdepth, functor, comparator);

			return true;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE void __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::Destroy(void)
		{
			m_nodes.clear();
		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE bool __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::Insert(const Object & object)
		{

			using namespace mye::math;

			AABB aabb = BoundingAABB(m_functor(object));

			if (!GetAABB().Contains(aabb))
			{
				return false;
			}

			MortonCode octant = __FindFittingOctant(aabb);

			__NodeHashMapIterator it = __CreateOctant(octant);

			it->second.objects.push_back(object);
			
			return true;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
			__MYE_ALGORITHMS_INLINE bool __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::Remove(const Object & object)
		{

			using namespace mye::math;

			AABB aabb = BoundingAABB(m_functor(object));

			MortonCode octant = __FindFittingOctant(aabb);

			auto it = m_nodes.find(octant);

			if (it != m_nodes.end())
			{

				__Node & node = it->second;

				std::list<Object>::iterator listIt;

				for (listIt = node.objects.begin(); listIt != node.objects.end(); listIt++)
				{

					if (m_comparator(*listIt, object))
					{
						node.objects.erase(listIt);
						return true;
					}

				}

			}

			return false;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		template <typename QueryType, typename Visitor>
		__MYE_ALGORITHMS_INLINE void __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::Query(const QueryType & query, Visitor visitor)
		{
			
			AABB rootAABB = AABB::FromCenterHalfExtents(m_center, m_halfextent * 2);

			__Traverse(__MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX, rootAABB, query, visitor);

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
			__MYE_ALGORITHMS_INLINE void __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__Create(const mye::math::Vector3 & center,
			                                                                         float halfextent,
			                                                                         unsigned int maxdepth,
			                                                                         BoundingVolumeFunctor functor,
			                                                                         Comparator comparator)
		{

			m_center     = center;
			m_halfextent = halfextent;
			m_maxdepth   = maxdepth;
			m_functor    = functor;
			m_comparator = comparator;

			m_nodes.emplace(__MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX, __Node());

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		template <typename Visitor>
		__MYE_ALGORITHMS_INLINE void __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::TraverseAABBs(Visitor visitor)
		{

			using namespace mye::math;

			__DFS(__MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX,
				  [this, visitor] (__NodeHashMapIterator it)
			{

				AABB aabb = __GetOctantAABB(it->first);

				visitor(aabb);

			});

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE mye::math::AABB __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::GetAABB(void) const
		{
			return AABB::FromCenterHalfExtents(m_center, m_halfextent);
		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		typename __MYE_ALGORITHMS_INLINE __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__NodeHashMapIterator
			__MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__CreateOctant(MortonCode octantLocation)
		{

			using namespace mye::math;

			auto nodeInsertResult = m_nodes.emplace(octantLocation, __Node());

			__NodeHashMapIterator octant = nodeInsertResult.first;

			// If the node was created, initialize it and continue in the recursion
			if (nodeInsertResult.second)
			{

				MortonCode childCode = (octantLocation & 7);

				MortonCode parentLocation = octantLocation >> 3;
				__NodeHashMapIterator parent = __CreateOctant(parentLocation);

				// Add the new node to the parent
				parent->second.childrenMask |= __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILD_MASK(childCode);

			}

			return octant;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE MortonCode __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__FindFittingOctant(const mye::math::AABB & aabb) const
		{

			using namespace mye::math;

			Vector3 center   = aabb.GetCenter();
			Vector3 hExtents = aabb.GetHalfExtents();

			Real maxExtent = Max(hExtents.x(), hExtents.y(), hExtents.z());

			Vector3 normalizedCentroid = (center - m_center + m_halfextent) / (2 * m_halfextent);

			// Given an object of size wx, we want a depth d such that wx <= wd, where wd = w0 / 2^d
			// Solving we have d <= log2 w0/wx. So d = floor(log2(w0/wx)) is the max depth an
			// object can fit in.

			unsigned int fitDepth = (unsigned int) Floor(std::log2(m_halfextent / maxExtent));
			unsigned int depth    = Min(fitDepth, m_maxdepth);

			Vector3 octreeCenterCoordinates = Floor(normalizedCentroid * (1 << m_maxdepth));

			MortonCode centerLocation = MortonEncode3(octreeCenterCoordinates.x(), octreeCenterCoordinates.y(), octreeCenterCoordinates.z());

			// Build the octant location code with the sentinel value
			MortonCode location = (centerLocation | (1 << (3 * m_maxdepth))) >> (3 * (m_maxdepth - depth));

			return location;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE unsigned int __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__GetOctantDepth(MortonCode location) const
		{

			// Find the most significant one (the sentinel value) and
			// return the number of bit triplets in the code as the depth value

			uint32_t mso = detail::MostSignificantOne(location);
			return mso / 3;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE mye::math::AABB __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__GetOctantAABB(MortonCode location) const
		{

			using namespace mye::math;

			if (location == __MYE_ALGORITHMS_LOOSEOCTREE_ROOT_INDEX)
			{
				return GetAABB();
			}

			auto it = m_nodes.find(location);

			if (it != m_nodes.end())
			{

				unsigned int depth = __GetOctantDepth(location);

				// The half extents of the regular octant
				float hd = m_halfextent / (1 << depth);
				float hdloose = 2.0f * hd;

				Vector3u octantCoords = __GetOctantCoordinates(location);

				Vector3 ijk = (Vector3(2) * Vector3(octantCoords.x(), octantCoords.y(), octantCoords.z()) + Vector3(1));

				Vector3 center = ijk * hd + (m_center - m_halfextent);

				return AABB::FromCenterHalfExtents(center, hdloose);

			}
			
			return AABB::FromCenterHalfExtents(0, 0);

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		__MYE_ALGORITHMS_INLINE mye::math::Vector3u __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__GetOctantCoordinates(MortonCode location) const
		{

			uint32_t mso = detail::MostSignificantOne(location);

			// Remove the sentinel
			MortonCode code = (1 << mso) ^ location;

			mye::math::Vector3u v;
			MortonDecode3(code, v.x(), v.y(), v.z());

			return v;

		}

		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		template <typename QueryType, typename Visitor>
		__MYE_ALGORITHMS_INLINE void __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__Traverse(MortonCode currentLocation,
																				   const mye::math::AABB & current,
																				   const QueryType & query,
																				   Visitor visitor)
		{

			using namespace mye::math;

			if (Intersect(current, query))
			{

				auto it = m_nodes.find(currentLocation);

				for (Object & o : it->second.objects)
				{
					visitor(o);
				}

				auto currentCenter      = current.GetCenter();
				auto currentHalfExtents = current.GetHalfExtents().x();

				auto nextHalfExtents = currentHalfExtents * 0.5f;

				for (int child = 0; child < 8; child++)
				{

					MortonCode nextLevelLocation = currentLocation << 3;

					if (it->second.childrenMask & __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILD_MASK(child))
					{

						MortonCode childCode = nextLevelLocation | child;

						Vector3 centerShift;

						float shiftSize = currentHalfExtents * 0.25f;

						if (child & 1)
						{
							centerShift.x() = shiftSize;
						}
						else
						{
							centerShift.x() = - shiftSize;
						}

						if (child & 2)
						{
							centerShift.y() = shiftSize;
						}
						else
						{
							centerShift.y() = - shiftSize;
						}

						if (child & 4)
						{
							centerShift.z() = shiftSize;
						}
						else
						{
							centerShift.z() = - shiftSize;
						}

						AABB childAABB = AABB::FromCenterHalfExtents(currentCenter + centerShift, nextHalfExtents);

						__Traverse(childCode, childAABB, query, visitor);

					}

				}

			}

		}

		
		__MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
		template <typename Visitor>
		__MYE_ALGORITHMS_INLINE void __MYE_ALGORITHMS_LOOSEOCTREE_TYPE::__DFS(MortonCode code, Visitor visitor)
		{

			auto it = m_nodes.find(code);

			visitor(it);

			MortonCode nextLevelLocation = code << 3;

			for (int child = 0; child < 8; child++)
			{

				if (it->second.childrenMask & __MYE_ALGORITHMS_LOOSEOCTREE_MAKE_CHILD_MASK(child))
				{
					__DFS(nextLevelLocation | child, visitor);
				}

			}

		}


	}

}

#undef __MYE_ALGORITHMS_LOOSEOCTREE_TEMPLATE_DECLARATION
#undef __MYE_ALGORITHMS_LOOSEOCTREE_TYPE