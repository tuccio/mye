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
			mye::math::AABB operator() (T &object);
		};

		/* Loose Octree Node */

		template <typename T>
		class LooseOctreeNode
		{

		public:
			
			class Iterator;

			LooseOctreeNode(LooseOctreeNode *parent,
				const mye::math::AABBf& bounds);

			~LooseOctreeNode(void);

			void Clear(void);

			inline mye::math::AABBf GetBounds(void) const;

			inline LooseOctreeNode* GetChild(OctreeChild child);

			inline unsigned int GetObjectsCount(void) const;

			inline void AddObject(const T &object);
			inline Iterator FindObject(const T &object);

			inline Iterator begin(void);
			inline Iterator end(void);

			MYE_DECLARE_POOL_ALLOCATOR(LooseOctreeNode<T>)

		private:
			
			template <typename T>
			friend class LooseOctreeTraverser;

			template <typename T>
			friend class LooseOctree;

			inline Iterator RemoveObject(const Iterator &it);

			LooseOctreeNode* m_children[8];
			LooseOctreeNode* m_parent;

			std::list<T> m_objects;
			mye::math::AABBf m_bounds;

		};

		template <typename T>
		class LooseOctreeNode<T>::Iterator
		{

		public:

			Iterator(const Iterator &it);

			inline T& operator* (void) const;
			inline bool operator!= (const Iterator &it) const;
			inline Iterator& operator++ (void);

		private:

			template <typename T>
			friend class LooseOctreeNode;

			Iterator(typename const std::list<T>::iterator &it);

			typename std::list<T>::iterator m_it;

		};

		/* Loose Octree */

		template <typename T>
		class LooseOctree
		{

		public:

			LooseOctree(mye::math::Vector3f center,
				float size,
				unsigned int maxdepth,
				unsigned int looseness);

			~LooseOctree(void);

			bool Insert(const T &object, const mye::math::AABBf &aabb);
			LooseOctreeNode<T>* Find(const mye::math::AABBf &aabb);
			LooseOctreeNode<T>* FindFirst(const mye::math::AABBf &aabb);
			typename LooseOctreeNode<T>::Iterator Find(const T &object, const const mye::math::AABBf &aabb);
			bool Remove(const T &object, const mye::math::AABBf &aabb);
			bool Relocate(const T &object, const mye::math::AABBf &oldAABB, const mye::math::AABBf &newAABB);

			LooseOctreeNode<T>* GetRoot(void);

		private:

			template <typename T>
			friend class LooseOctreeTraverser;

			mye::math::AABBf ChildBounds(LooseOctreeNode<T> *parent, OctreeChild child) const;
			OctreeChild ChooseChild(LooseOctreeNode<T> *parent, const mye::math::AABBf &aabb) const;

			inline bool Find(const mye::math::AABBf &aabb,
				LooseOctreeNode<T> **node,
				OctreeChild *child,
				unsigned int *depth,
				bool *needToAllocate);

			inline void AllocateChildren(const mye::math::AABBf &aabb,
				LooseOctreeNode<T> **node,
				OctreeChild *child,
				unsigned int *depth);

			LooseOctreeNode<T> m_root;
			unsigned int m_maxdepth;
			unsigned int m_looseness;

			mye::math::AABBf m_bounds;

		};

		/* Loose Octree Traverser */

		template <typename T>
		class LooseOctreeTraverser
		{

		public:

			LooseOctreeTraverser(LooseOctree<T> &octree);
			~LooseOctreeTraverser(void);

			inline LooseOctreeNode* GetCurrent(void);
			inline LooseOctreeNode* GetParent(void);

			inline void MoveToParent(void);
			inline bool MoveToChild(OctreeChild child);
			inline void MoveToRoot(void);

			inline unsigned int GetDepth(void) const;

		private:

			std::deque<LooseOctreeNode<T>*> m_path;
			LooseOctree<T> *octree;

		};

	}

}

#include "LooseOctree.inl"