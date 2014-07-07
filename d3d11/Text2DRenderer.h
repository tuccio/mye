#pragma once

#include "DX11Device.h"

#include "DX11ConstantBuffer.h"
#include "DX11DepthBuffer.h"
#include "DX11SwapChain.h"
#include "DX11Texture.h"

#include "DX11PixelShader.h"
#include "DX11VertexShader.h"

#include <mye/win/Window.h>

namespace mye
{

	namespace dx11
	{

		class Text2DRenderer 
		{

		public:

			Text2DRenderer(DX11Device &device);
			~Text2DRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView *target);

		private:

			bool                    m_initialized;

			DX11Device             &m_device;

			DX11VertexShaderPointer m_text2dVS;
			DX11PixelShaderPointer  m_text2dPS;

			ID3D11SamplerState     *m_fontTextureSampler;

			DX11ConstantBuffer      m_textColorBuffer;

			DX11SwapChain          *m_swapChain;

			bool CreateConstantBuffers(void);

		};

	}

}
