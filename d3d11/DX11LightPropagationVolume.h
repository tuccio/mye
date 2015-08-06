#pragma once

#include <mye/core/Game.h>

#include "DX11Device.h"
#include "DX11ShaderResource.h"
#include "DX11ReflectiveShadowMap.h"
#include "DX11Texture.h"
#include "DX11ConstantBuffer.h"

#include <mye/math/Math.h>

namespace mye
{

	namespace dx11
	{

		namespace detail
		{

			class LightVolume
			{

			public:

				LightVolume(void);

				bool Create(size_t resolution);
				void Destroy(void);

				void SetRenderTarget(void);

				void Bind(DX11PipelineStage stage, int slot);
				void Unbind(void);

				void Clear(const mye::math::Vector4f & color = mye::math::Vector4f(0.f));

			private:

				DX11Texture m_shR;
				DX11Texture m_shG;
				DX11Texture m_shB;

			};

		}

		class DX11LightPropagationVolume
		{

		public:

			DX11LightPropagationVolume(void);

			bool Create(size_t dimensions = 32,
			            size_t resolution = mye::core::Game::GetSingleton().GetGraphicsModule()->GetRendererConfiguration()->GetShadowMapResolution() / 4);

			void Destroy(void);

			void Init(const mye::core::Camera * camera, const mye::math::Vector3 & volumeMinCorner, const mye::math::Vector3 & volumeMaxCorner);
			void Inject(DX11ReflectiveShadowMap & rsm);
			void Propagate(size_t iterations);

		private:

			DX11VertexShaderPointer   m_lpvRSMSamplerVS;
			DX11PixelShaderPointer    m_lpvRSMSamplerPS;
								     
			DX11Texture               m_position;
			DX11Texture               m_normal;
			DX11Texture               m_flux;
			DX11Texture               m_depth;
								      
			detail::LightVolume       m_lightVolume[2];
			DX11Texture               m_geometryVolume;
							         
			size_t                    m_sampleResolution;
			size_t                    m_volumeResolution;
							         
			mye::math::Vector3        m_volumeMinCorner;
			mye::math::Vector3        m_volumeMaxCorner;
			mye::math::Real           m_volumeCellSize;
							         
			ID3D11SamplerState      * m_linearSampler;
							         
			DX11ConstantBuffer        m_lightBuffer;
			DX11ConstantBuffer        m_cameraTransformBuffer;
			DX11ConstantBuffer        m_lpvInfo;

			uint8_t                   m_currentVolume;

		};

	}

}