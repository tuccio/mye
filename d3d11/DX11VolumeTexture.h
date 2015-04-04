#pragma once

#include "DX11Device.h"
#include "DX11ShaderResource.h"
#include "DX11RenderTarget.h"

namespace mye
{

	namespace dx11
	{

		class DX11VolumeTexture :
			public DX11ShaderResource,
			public DX11RenderTarget
		{

		public:

			DX11VolumeTexture(DX11Device & device);

			bool Create(int width, int height, int depth, mye::core::DataFormat format);
			void Destroy(void);

		private:

			DX11Device      & m_device;
			ID3D11Texture3D * m_texture;

		};

	}

}