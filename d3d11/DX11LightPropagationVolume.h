#pragma once

#include <mye/core/Game.h>

#include "DX11Device.h"
#include "DX11ShaderResource.h"
#include "DX11ReflectiveShadowMap.h"
#include "DX11Texture.h"
#include "DX11ConstantBuffer.h"
#include "DX11ShaderProgram.h"
#include "DX11VertexBuffer.h"

#include <mye/math/Math.h>
#include <array>

namespace mye
{

	namespace dx11
	{

		class DX11LightVolume
		{

		public:

			DX11LightVolume(void);

			bool Create(size_t resolution);
			void Destroy(void);

			std::array<ID3D11RenderTargetView *, 3> GetRenderTargetViews(void);
			void SetRenderTarget(void);

			void Bind(DX11PipelineStage stage, int redSlot, int greenSlot, int blueSlot);
			void Unbind(void);

			void Clear(const mye::math::Vector4f & color = mye::math::Vector4f(0.f));

		private:

			DX11Texture m_shR;
			DX11Texture m_shG;
			DX11Texture m_shB;

		};

		class DX11GeometryVolume
		{

		public:

			DX11GeometryVolume(void);

			bool Create(size_t resolution);
			void Destroy(void);

			ID3D11RenderTargetView * GetRenderTargetView(void);
			void SetRenderTarget(void);

			void Bind(DX11PipelineStage stage, int startSlot);
			void Unbind(void);

			void Clear(const mye::math::Vector4f & color = mye::math::Vector4f(0.f));

		private:

			DX11Texture m_geometryVolume;

		};

		class DX11LightPropagationVolume
		{

		public:

			DX11LightPropagationVolume(void);

			bool Create(size_t dimensions = 32,
			            size_t rsmSamples = 256);

			void Destroy(void);

			void Init(const mye::core::Camera * camera, const mye::math::Vector3 & volumeMinCorner, const mye::math::Vector3 & volumeMaxCorner);
			void Inject(DX11ReflectiveShadowMap & rsm);
			void Propagate(unsigned int iterations);

			void BindConfigurationBuffer(DX11PipelineStage stage, unsigned int slot);

			inline DX11LightVolume    & GetLightVolume(void)    { return m_lightVolume[m_currentVolume]; }
			inline DX11GeometryVolume & GetGeometryVolume(void) { return m_geometryVolume; }

			mye::math::Real GetGeometryInjectionBias(void) const;
			void SetGeometryInjectionBias(mye::math::Real bias);

			mye::math::Real GetFluxInjectionBias(void) const;
			void SetFluxInjectionBias(mye::math::Real bias);

			void   SetRSMSamples(size_t samples);
			size_t GetRSMSamples(void) const;

		private:

			DX11ShaderProgramPointer    m_lpvRSMSampling;
			DX11ShaderProgramPointer    m_lpvInjectFlux;
			DX11ShaderProgramPointer    m_lpvInjectGeometry;
			DX11ShaderProgramPointer    m_lpvPropagate;
			DX11ShaderProgramPointer    m_lpvPropagateFirst;
								     
			DX11Texture                 m_position;
			DX11Texture                 m_normal;
			DX11Texture                 m_flux;
			DX11Texture                 m_depth;
								      
			DX11LightVolume             m_lightVolume[2];
			DX11GeometryVolume          m_geometryVolume;
							         
			size_t                      m_rsmSamples;
			size_t                      m_volumeResolution;
							         
			mye::math::Vector3          m_volumeMinCorner;
			mye::math::Vector3          m_volumeMaxCorner;
			mye::math::Real             m_volumeCellSize;

			ID3D11BlendState          * m_additiveBlend;
							         
			DX11ConstantBuffer          m_lightBuffer;
			DX11ConstantBuffer          m_lpvConfig;

			DX11VertexBufferPointer     m_quadVertexBuffer;;

			uint8_t                     m_currentVolume;

			mye::math::Real             m_geometryInjectionBias;
			mye::math::Real             m_fluxInjectionBias;

			/* Private functions */

			bool __CreateShaders(void);
			void __DestroyShaders(void);

			bool __CreateBuffers(void);
			void __DestroyBuffers(void);

			bool __CreateContextStates(void);
			void __DestroyContextStates(void);

		};

	}

}