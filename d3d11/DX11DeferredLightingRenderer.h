#pragma once

#include "DX11Device.h"

#include "DX11ConstantBuffer.h"
#include "DX11DepthBuffer.h"
#include "DX11SwapChain.h"
#include "DX11Texture.h"
#include "DX11RenderTarget.h"
#include "DX11ReflectiveShadowMap.h"

#include "DX11PixelShader.h"
#include "DX11VertexShader.h"

#include <mye/core/RendererConfiguration.h>
#include <mye/win/Window.h>

namespace mye
{

	namespace dx11
	{

		class DX11DeferredLightingRenderer :
			public mye::core::IWindow::Listener,
			public mye::core::RendererConfigurationListener
		{

		public:

			DX11DeferredLightingRenderer(DX11Device & device, mye::win::Window * window);
			~DX11DeferredLightingRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView * target);

			void SetWindow(mye::win::Window * window);
			void OnResize(mye::core::IWindow * window, const mye::math::Vector2i & size);

			void OnConfigurationChange(mye::core::RendererVariable variable, mye::core::RendererConfiguration * configuration);

			inline DX11DepthBuffer & GetDepthBuffer(void)
			{
				return m_depthBuffer;
			}

		private:

			bool                      m_initialized;
								    
			DX11Device              & m_device;
			mye::win::Window        * m_window;							     
								    
			DX11VertexShaderPointer   m_deferredGeometryVS;
			DX11PixelShaderPointer    m_deferredGeometryPS;
								      
			DX11VertexShaderPointer   m_deferredLightsVS;
			DX11PixelShaderPointer    m_deferredLightsPS;
								      
			DX11VertexShaderPointer   m_deferredFinalVS;
			DX11PixelShaderPointer    m_deferredFinalPS;
								      
			DX11Texture               m_gbuffer0;
			DX11Texture               m_gbuffer1;
			DX11Texture               m_lbuffer;
								      
			DX11ReflectiveShadowMap   m_rsm;
								      
			DX11ConstantBuffer        m_transformBuffer;
			DX11ConstantBuffer        m_lightBuffer;
			DX11ConstantBuffer        m_cameraBuffer;
			DX11ConstantBuffer        m_materialBuffer;
			DX11ConstantBuffer        m_configurationBuffer;
								      
			DX11DepthBuffer           m_depthBuffer;
								    
			ID3D11SamplerState      * m_shadowMapSamplerState;
			ID3D11SamplerState      * m_shadowMapSamplerCmpState;
								    
			mye::math::Vector4f       m_clearColor;

			/* Private methods */

			bool CreateConstantBuffers(void);
			void DestroyConstantBuffers(void);

			bool CreateSamplerStates(void);
			void DestroySamplerStates(void);

		};

	}

}
