#pragma once

#include "DX11Font.h"
#include <mye/core/ResourceManager.h>

namespace mye
{

	namespace dx11
	{

		class DX11FontManager :
			public mye::core::ResourceManager
		{

		public:

			DX11FontManager(void);
			~DX11FontManager(void);

		protected:

			DX11Font * CreateImpl(const mye::core::String         & name,
			                      mye::core::ManualResourceLoader * manual,
			                      const mye::core::Parameters     & params);

			void FreeImpl(mye::core::Resource * resource);

		};

	}

}



