#pragma once

#include "DX11Device.h"
#include "DX11ShaderResource.h"

namespace mye
{

	namespace dx11
	{

		struct DX11DepthBufferConfiguration
		{

			DX11Device * device;

			int width;
			int height;

			bool shaderResource;

		};

		class DX11DepthBuffer :
			public DX11ShaderResource
		{

		public:

			DX11DepthBuffer(void);
			DX11DepthBuffer(DX11DepthBufferConfiguration depthBufferConfiguration);

			~DX11DepthBuffer(void);

			bool Create(void);
			void Destroy(void);

			void Resize(int width, int height);

			void Clear(float depth = 1.0f);

			inline ID3D11DepthStencilView * GetDepthStencilView(void)
			{
				return m_depthStencilView;
			}

		private:

			ID3D11Texture2D              * m_depthStencilBuffer;
			ID3D11DepthStencilView       * m_depthStencilView;
			DX11DepthBufferConfiguration   m_depthBufferConfiguration;

		};

	}
}
