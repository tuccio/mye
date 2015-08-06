#pragma once

#include <mye/core/Font.h>

#include "DX11Texture.h"

namespace mye
{

	namespace dx11
	{

		class DX11Font :
			public mye::core::Font
		{

		public:

			DX11Font(mye::core::ResourceManager      * owner,
			         const mye::core::String         & name,
			         mye::core::ManualResourceLoader * manual);

			~DX11Font(void);

			void Bind(void);
			void Unbind(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			DX11TexturePointer m_texture;

		};

		typedef std::shared_ptr<DX11Font> DX11FontPointer;

	}

}

