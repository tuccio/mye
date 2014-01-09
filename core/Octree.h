#pragma once

namespace mye
{

	namespace core
	{

		class OctreeNode
		{

		public:

			OctreeNode(void);
			~OctreeNode(void);

		private:

			OctreeNode *m_children;

		};

		class Octree
		{

		public:

			Octree(void);
			~Octree(void);

		};

	}

}
