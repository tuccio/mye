#pragma once

#include <mye/core/ResourceManager.h>

#include "DX11Texture.h"

namespace mye
{

	namespace dx11
	{

		class DX11TextureManager :
			public mye::core::ResourceManager
		{

		public:

			DX11TextureManager(void);
			~DX11TextureManager(void);

			DX11Texture * CreateImpl(const mye::core::String         & name,
			                         mye::core::ManualResourceLoader * manual,
			                         const mye::core::Parameters     & params);

			void FreeImpl(mye::core::Resource * resource);

		};

	}

}
