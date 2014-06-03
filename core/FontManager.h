#pragma once

#include "ResourceManager.h"
#include "Font.h"

namespace mye
{

	namespace core
	{

		class FontManager :
			public ResourceManager
		{

		public:

			FontManager(void);
			~FontManager(void);

		protected:

			Font* CreateImpl(const String &name,
				ManualResourceLoader *manual,
				const Parameters &params);

			void FreeImpl(Resource *resource);

		};

	}

}
