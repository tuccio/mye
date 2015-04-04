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

		class DX11DeferredShadingRenderer :
			public mye::core::IWindow::Listener
		{

		public:

			DX11DeferredShadingRenderer(DX11Device & device, mye::win::Window * window);
			~DX11DeferredShadingRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView * target);

			void SetWindow(mye::win::Window * window);
			void OnResize(mye::core::IWindow * window, const mye::math::Vector2i & size);

		private:			

			bool                      m_initialized;
								      
			DX11Device              & m_device;
								      
			mye::win::Window        * m_window;
								      
			DX11VertexShaderPointer   m_deferredGeometryVS;
			DX11PixelShaderPointer    m_deferredGeometryPS;
								      
			DX11VertexShaderPointer   m_deferredLightsVS;
			DX11PixelShaderPointer    m_deferredLightsPS;
								      
			DX11Texture               m_target0;
			DX11Texture               m_target1;
			DX11Texture               m_target2;
			DX11Texture               m_target3;
							          
			DX11ConstantBuffer        m_transformBuffer;
			DX11ConstantBuffer        m_lightBuffer;
			DX11ConstantBuffer        m_cameraBuffer;
			DX11ConstantBuffer        m_materialBuffer;
							          
			DX11DepthBuffer           m_depthBuffer;
								      
			mye::math::Vector4f       m_clearColor;

			bool CreateConstantBuffers(void);

		};

	}

}
