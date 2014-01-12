#pragma once

#include "SceneGraphNode.h"

namespace mye
{

	namespace core
	{

		class SceneGraph
		{

		public:

			SceneGraph(void);
			~SceneGraph(void);

		private:

			WorldGraphNode m_root;

		};

	}

}
