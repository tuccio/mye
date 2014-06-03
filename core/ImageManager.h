#pragma once

#include "ResourceManager.h"
#include "Image.h"

namespace mye
{

	namespace core
	{

		class ImageManager :
			public ResourceManager
		{

		public:

			ImageManager(void);
			~ImageManager(void);

			Image* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				const Parameters &params);

			void FreeImpl(Resource *resource);

		};

	}

}
