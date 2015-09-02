#pragma once

#include "DX11Device.h"

#include "DX11ConstantBuffer.h"
#include "DX11DepthBuffer.h"
#include "DX11SwapChain.h"
#include "DX11Texture.h"
#include "DX11RenderTarget.h"
#include "DX11ReflectiveShadowMap.h"
#include "DX11LightPropagationVolume.h"
#include "DX11VertexBuffer.h"

#include "DX11PixelShader.h"
#include "DX11VertexShader.h"
#include "DX11ShaderProgram.h"

#include <mye/core/EventManager.h>
#include <mye/core/RendererConfiguration.h>

#include <boost/atomic.hpp>

namespace mye
{

	namespace dx11
	{

		class DX11DeferredLightingRenderer :
			public mye::core::IEventListener
		{

		public:

			DX11DeferredLightingRenderer(void);
			~DX11DeferredLightingRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView * target);

			void OnEvent(const mye::core::IEvent * event);

			inline DX11DepthBuffer & GetDepthBuffer(void)
			{
				return m_depthBuffer;
			}

		private:

			bool                         m_initialized;

			DX11ShaderProgramPointer     m_deferredGeometry[2];
			DX11ShaderProgramPointer     m_deferredLights;
			DX11ShaderProgramPointer     m_deferredLightsLPV;
			DX11ShaderProgramPointer     m_deferredFinal[2];
								         
			DX11Texture                  m_gbuffer0;
			DX11Texture                  m_gbuffer1;
			DX11Texture                  m_lbuffer;
								         
			DX11ReflectiveShadowMap      m_rsm;
			DX11LightPropagationVolume   m_lpv;
								      
			DX11ConstantBuffer           m_transformBuffer;
			DX11ConstantBuffer           m_lightBuffer;
			DX11ConstantBuffer           m_materialBuffer;
			DX11ConstantBuffer           m_configurationBuffer;
			DX11ConstantBuffer           m_cameraTransformBuffer;
			DX11ConstantBuffer           m_matrixBuffer;
			DX11ConstantBuffer           m_pssmSliceBuffer;

			DX11VertexBufferPointer      m_quadVertexBuffer;
								         
			DX11DepthBuffer              m_depthBuffer;
			
			ID3D11SamplerState         * m_linearSamplerState;
			ID3D11SamplerState         * m_lpvSamplerState;
			ID3D11SamplerState         * m_randomSamplerState;
			ID3D11SamplerState         * m_shadowMapSamplerState;
			ID3D11SamplerState         * m_shadowMapSamplerCmpState;
			ID3D11SamplerState         * m_anisotropicSampler;

			ID3D11BlendState           * m_accumulateBlendState;
									   
			DX11TexturePointer           m_randomCosSin;
								       
			mye::math::Vector4f          m_clearColor;
									   
			boost::atomic<bool>          m_renderConfigurationUptodate;

			/* Private methods */

			void __ResizeBuffers(const mye::math::Vector2i & size);

			bool __CreateConstantBuffers(void);
			void __DestroyConstantBuffers(void);

			bool __CreateContextStates(void);
			void __DestroyContextStates(void);

			void __UpdateRenderConfiguration(void);

		};

	}

}
