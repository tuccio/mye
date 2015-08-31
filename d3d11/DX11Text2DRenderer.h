#pragma once

#include "DX11Device.h"

#include "DX11ConstantBuffer.h"
#include "DX11DepthBuffer.h"
#include "DX11SwapChain.h"
#include "DX11Texture.h"

#include "DX11ShaderProgram.h"

#include <mye/win/Window.h>

namespace mye
{

	namespace dx11
	{

		class DX11Text2DRenderer 
		{

		public:

			DX11Text2DRenderer(void);
			~DX11Text2DRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView * target);

		private:

			bool                      m_initialized;
								      
			DX11ShaderProgramPointer  m_text2d;
								      
			ID3D11SamplerState      * m_fontTextureSampler;
								      
			DX11ConstantBuffer        m_textColorBuffer;

			bool __CreateConstantBuffers(void);

		};

	}

}
