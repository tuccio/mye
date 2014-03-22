#pragma once

#include <mye/math/Geometry.h>

#include <list>
#include <deque>
#include <cassert>

#include "PoolAllocator.h"

namespace mye
{

	namespace core
	{

		enum class OctreeChild
		{
			LEFT_BOTTOM_NEAR,
			LEFT_BOTTOM_FAR,
			LEFT_TOP_NEAR,
			LEFT_TOP_FAR,
			RIGHT_BOTTOM_NEAR,
			RIGHT_BOTTOM_FAR,
			RIGHT_TOP_NEAR,
			RIGHT_TOP_FAR,
			NONE
		};

		template <typename T>
		struct LooseOctreeSpacing
		{
			mye::math::AABBt operator() (T &object);
		};

		/* Loose Octree Node */

		template <typename T, typename BoundingVolume = mye::math::AABB>
		class LooseOctreeNode
		{

		public:
			
			typedef LooseOctreeNode<T, BoundingVolume> Type;
			class Iterator;

			LooseOctreeNode(LooseOctreeNode *parent,
				const mye::math::AABB& bounds);

			~LooseOctreeNode(void);

			void Clear(void);

			inline mye::math::AABB GetBounds(void) const;

			inline LooseOctreeNode* GetChild(OctreeChild child);

			inline unsigned int GetObjectsCount(void) const;

			inline void AddObject(const T &object);
			inline Iterator FindObject(const T &object);

			inline Iterator begin(void);
			inline Iterator end(void);

			MYE_DECLARE_POOL_ALLOCATOR(Type)

		private:
			
			template <typename T, typename BoundingVolume>
			friend class LooseOctreeTraverser;

			template <typename T, typename BoundingVolume>
			friend class LooseOctree;

			inline Iterator RemoveObject(const Iterator &it);

			LooseOctreeNode* m_children[8];
			LooseOctreeNode* m_parent;

			std::list<T> m_objects;
			mye::math::AABB m_bounds;

		};

		template <typename T, typename BoundingVolume>
		class LooseOctreeNode<T, BoundingVolume>::Iterator
		{

		public:

			Iterator(const Iterator &it);

			inline T& operator* (void) const;
			inline bool operator!= (const Iterator &it) const;
			inline Iterator& operator++ (void);

		private:

			template <typename T, typename BoundingVolume>
			friend class LooseOctreeNode;

			Iterator(typename const std::list<T>::iterator &it);

			typename std::list<T>::iterator m_it;

		};

		/* Loose Octree */

		template <typename T, typename BoundingVolume = mye::math::AABB>
		class LooseOctree
		{

		public:

			LooseOctree(mye::math::Vector3 center,
				float size,
				unsigned int maxdepth,
				unsigned int looseness);

			~LooseOctree(void);

			bool Insert(const T &object, const BoundingVolume &boundingVolume);
			LooseOctreeNode<T, BoundingVolume>* Find(const BoundingVolume &boundingVolume);
			LooseOctreeNode<T, BoundingVolume>* FindFirst(const BoundingVolume &boundingVolume);
			typename LooseOctreeNode<T, BoundingVolume>::Iterator Find(const T &object, const const BoundingVolume &boundingVolume);
			bool Remove(const T &object, const BoundingVolume &boundingVolume);
			bool Relocate(const T &object, const BoundingVolume &oldboundingVolume, const BoundingVolume &newboundingVolume);

			LooseOctreeNode<T, BoundingVolume>* GetRoot(void);

			void Clear(void);

		private:

			template <typename T, typename BoundingVolume>
			friend class LooseOctreeTraverser;

			mye::math::AABB ChildBounds(LooseOctreeNode<T, BoundingVolume> *parent, OctreeChild child) const;
			OctreeChild ChooseChild(LooseOctreeNode<T, BoundingVolume> *parent, const BoundingVolume &boundingVolume) const;

			inline bool Find(const BoundingVolume &boundingVolume,
				LooseOctreeNode<T, BoundingVolume> **node,
				OctreeChild *child,
				unsigned int *depth,
				bool *needToAllocate);

			inline void AllocateChildren(const BoundingVolume &AABBt,
				LooseOctreeNode<T, BoundingVolume> **node,
				OctreeChild *child,
				unsigned int *depth);

			LooseOctreeNode<T> m_root;
			unsigned int m_maxdepth;
			unsigned int m_looseness;

			mye::math::AABB m_bounds;

		};

		/* Loose Octree Traverser */

		template <typename T, typename BoundingVolume = mye::math::AABB>
		class LooseOctreeTraverser
		{

		public:

			LooseOctreeTraverser(LooseOctree<T, BoundingVolume> &octree);
			~LooseOctreeTraverser(void);

			inline LooseOctreeNode* GetCurrent(void);
			inline LooseOctreeNode* GetParent(void);

			inline void MoveToParent(void);
			inline bool MoveToChild(OctreeChild child);
			inline void MoveToRoot(void);

			inline unsigned int GetDepth(void) const;

		private:

			std::deque<LooseOctreeNode<T, BoundingVolume>*> m_path;
			LooseOctree<T, BoundingVolume> *octree;

		};

	}

}

#include "LooseOctree.inl"