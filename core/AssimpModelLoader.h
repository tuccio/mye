#pragma once

#include "Resource.h"
#include "ManualResourceLoader.h"

#include <assimp/scene.h>

namespace mye
{

	namespace core
	{

		class AssimpModelLoader :
			public ManualResourceLoader
		{

		public:

			AssimpModelLoader(void);
			AssimpModelLoader(const aiScene *scene);
			~AssimpModelLoader(void);

			bool Load(Resource *resource);
			void Unload(Resource *resource);

		private:

			const aiScene *m_scene;

		};

	}

}
