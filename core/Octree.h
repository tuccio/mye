#pragma once

#include "PoolAllocator.h"

#include <mye/math/Geometry.h>

#include <deque>
#include <list>
#include <algorithm>

namespace mye
{

	namespace core
	{

		class OctreeNode
		{

		public:

			OctreeNode(bool leaf);
			
			inline bool IsLeaf(void) const;

		protected:

			bool m_leaf;

		};

		template <typename T>
		struct OctreePositionGrabber
		{
			mye::math::Vector3f operator() (T &o);
		};

		template <>
		struct OctreePositionGrabber<mye::math::Vector3f>
		{
			inline mye::math::Vector3f operator() (mye::math::Vector3f &v);
		};

		template <>
		struct OctreePositionGrabber<mye::math::Transformf>
		{
			inline mye::math::Vector3f operator() (mye::math::Transformf &t);
		};

		template <typename T>
		class OctreeLeaf :
			public OctreeNode
		{

		public:

			OctreeLeaf(void);

			inline typename std::vector<T>::iterator begin(void);
			inline typename std::vector<T>::iterator end(void);

// 			void AddObject(T &object);
// 			void RemoveObject(T &object);

		private:

			template <typename T>
			friend class Octree;

			std::vector<T> m_objects;

		};

		class OctreeInternalNode :
			public OctreeNode
		{

		public:

			enum Children
			{
				FRONT_LEFT_BOTTOM = 0,
				FRONT_RIGHT_BOTTOM = 1,
				FRONT_RIGHT_TOP = 2,
				FRONT_LEFT_TOP = 3,
				BACK_LEFT_TOP = 4,
				BACK_RIGHT_TOP = 5,
				BACK_RIGHT_BOTTOM = 6,
				BACK_LEFT_BOTTOM = 7
			};

			OctreeInternalNode(void);
			~OctreeInternalNode(void);

			void Clear(void);

			inline const OctreeNode* GetChild(Children child) const;
			inline OctreeNode* GetChild(Children child);

		private:

			template <typename T>
			friend class Octree;

			OctreeNode* m_children[8];
			unsigned char m_childrenCount;

		};

		template <typename T>
		class OctreeTraverser;

		template <typename T>
		class Octree
		{

		public:

			Octree(const mye::math::Vector3f &center,
				const mye::math::Vector3f &size,
				unsigned int maxDepth);

			~Octree(void);

			void Clear(void);

			void Insert(T &object);

			void Relocate(T &object,
				const mye::math::Vector3f &xOld);
			
			inline bool Remove(T &object);
			bool Remove(T &object, const mye::math::Vector3f &x);

			OctreeTraverser<T> Traverse(const mye::math::Vector3f &x);

			inline const OctreeInternalNode* GetRoot(void) const;
			inline OctreeInternalNode* GetRoot(void);

			const mye::math::AABB& GetBounds(void) const;

			unsigned int GetMaxDepth(void) const;

		private:

			OctreeInternalNode m_root;
			mye::math::AABB m_bounds;
			unsigned int m_maxDepth;

		};

		template <typename T>
		class OctreeTraverser
		{

		public:

			OctreeTraverser(Octree<T> &octree);

			inline Octree<T>& GetOctree(void);
			inline const Octree<T>& GetOctree(void) const;

			inline const mye::math::AABB& GetBounds(void) const;

			inline OctreeNode* GetCurrent(void);
			inline const OctreeNode* GetCurrent(void) const;

			inline OctreeInternalNode* GetParent(void);
			inline const OctreeInternalNode* GetParent(void) const;

			inline OctreeInternalNode::Children GetChildSide(void) const;

			inline bool Traverse(const mye::math::Vector3f &x);

			inline bool MoveToChild(OctreeInternalNode::Children child);
			inline OctreeInternalNode* MoveToParent(void);
			inline OctreeInternalNode* MoveToRoot(void);

			inline unsigned int GetDepth(void) const;

		private:

			struct NodeInfo
			{
				OctreeNode *node;
				OctreeInternalNode::Children child;
			};

			Octree<T> *m_octree;
			std::deque<NodeInfo> m_path;
			mye::math::AABB m_bounds;

		};

		inline OctreeInternalNode::Children __PickSide(
			const mye::math::AABB &aabb,
			const mye::math::Vector3f &x);

		inline mye::math::AABB __SplitAABB(
			const mye::math::AABB &aabb,
			OctreeInternalNode::Children child);

		inline mye::math::AABB __InverseSplitAABB(
			const mye::math::AABB &aabb,
			OctreeInternalNode::Children child);

	}

}

#include "Octree.inl"