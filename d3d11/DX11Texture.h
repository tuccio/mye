#pragma once

#include <mye/core/Image.h>
#include <mye/core/Texture.h>

#include <d3d11.h>

#include "DX11Device.h"

namespace mye
{

	namespace dx11
	{

		/*
		 * Params:
		 *
		 *    int    levels       : Mimap levels
		 *    bool   multisample  : Antialiasing setting
		 *    string usage        : [default/static/dynamic/stream] -> [rw gpu/r gpu/r gpu w cpu/rw gpu cpu]
		 *    boool  generateMips : Mipmap generation setting
		 *
		 */

		class DX11Texture :
			public mye::core::Texture
		{

		public:

			DX11Texture(mye::core::ResourceManager *owner,
				const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				DX11Device &device);

			~DX11Texture(void);

			void Bind(unsigned int slot);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		private:

			DX11Device      &m_device;
			ID3D11Texture2D *m_texture;

			ID3D11ShaderResourceView *m_shaderResourceView;
			

		};

		typedef boost::shared_ptr<DX11Texture> DX11TexturePointer;

	}

}

