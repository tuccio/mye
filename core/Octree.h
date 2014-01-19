#pragma once

#include <Eigen/Eigen>

namespace mye
{

	namespace core
	{

		template <typename T>
		class OctreeNode
		{

		public:

			OctreeNode(void);
			~OctreeNode(void);

		private:

			OctreeNode *m_children[8];

		};

		template <typename T>
		class Octree
		{

		public:

			Octree(void);
			~Octree(void);

		private:

			Eigen::Vector3f m_halfExtents;
			Eigen::Vector3f m_center;

		};

	}

}

#include "Octree.inl"