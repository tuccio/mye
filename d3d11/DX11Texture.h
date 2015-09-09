#pragma once

#include <mye/core/Image.h>
#include <mye/core/Texture.h>
#include <mye/core/VertexDeclaration.h>

#include <d3d11.h>

#include "DX11Device.h"
#include "DX11RenderTarget.h"
#include "DX11ShaderResource.h"

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
		 *    bool   generateMips : Mipmap generation setting
		 *    int    mipmaps      : Number of mipmaps
		 *    int    msaa         : Number of msaa samples
		 *
		 */

		class DX11Texture :
			public mye::core::Texture,
			public DX11RenderTarget,
			public DX11ShaderResource
		{

		public:

			DX11Texture(mye::core::ResourceManager * owner,
						const mye::core::String & name,
						mye::core::ManualResourceLoader * manual);

			DX11Texture(void);

			~DX11Texture(void);

			bool Create(int width, int height, mye::core::DataFormat format, void * data = nullptr);
			void Destroy(void);

			void GenerateMips(void);

			ID3D11Texture2D * GetTexture2D(void) const;
			ID3D11Texture3D * GetTexture3D(void) const;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

			bool CreateViews(void);

			union
			{
				ID3D11Texture2D * m_texture2d;
				ID3D11Texture3D * m_texture3d;
			};

			D3D11_SRV_DIMENSION   m_textureType;

			mye::core::DataFormat m_format;
			MSAA                  m_msaa;

		private:

			void SetMSAA(void);

		};

		typedef std::shared_ptr<DX11Texture> DX11TexturePointer;

	}

}

