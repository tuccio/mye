#pragma once

#include "DX11DepthBuffer.h"
#include "DX11Texture.h"
#include "DX11VertexShader.h"
#include "DX11GeometryShader.h"
#include "DX11PixelShader.h"
#include "DX11ConstantBuffer.h"

#include <mye/core/Camera.h>
#include <mye/core/Light.h>
#include <mye/math/Math.h>
#include <mye/core/SceneModule.h>

#undef near
#undef far

#define __MYE_RSM_MAX_CSM_COUNT 8

namespace mye
{

	namespace dx11
	{

		class DX11ReflectiveShadowMap
		{

		public:

			struct PSSMSlice
			{
				mye::math::Real near;
				mye::math::Real far;
			};

			DX11ReflectiveShadowMap(void);

			bool Create(void);
			void Destroy(void);

			void Render(mye::core::Light * light);

			void Bind(DX11PipelineStage stage, int slice = 0);
			void Unbind(void);

			inline int GetResolution(void) const
			{
				return m_resolution;
			}

			void SetResolution(int resolution);

			inline int GetCSMSplits(void) const
			{
				return m_csmSplits;
			}

			void SetCSMSplits(int slisplitsces);

			inline float GetCSMLogarithmicWeight(void) const
			{
				return m_csmLogWeight;
			}

			inline void SetCSMLogarithmicWeight(float weight)
			{
				m_csmLogWeight = weight;
			}

			inline DX11ShaderResource & GetPositionShaderResource(void)
			{
				return m_position;
			}

			inline DX11ShaderResource & GetNormalShaderResource(void)
			{
				return m_normal;
			}

			inline DX11ShaderResource & GetFluxShaderResource(void)
			{
				return m_flux;
			}

			inline DX11ShaderResource & GetDepthShaderResource(void)
			{
				return m_depth;
			}

			inline const mye::math::Matrix4 & GetLightSpaceTransformMatrix(void)
			{
				return m_lightSpaceTransform;
			}

			inline const mye::math::Matrix4 & GetPSSMCropMatrix(int slice)
			{
				return m_cropMatrix[slice];
			}

			inline const std::vector<PSSMSlice> & GetPSSMSlices(void) const
			{
				return m_pssmSlices;
			}

			inline mye::core::Light * GetLight(void) const
			{
				return m_light;
			}

		private:

			bool                        m_initialized;

			DX11Texture                 m_position;
			DX11Texture                 m_normal;
			DX11Texture                 m_flux;

			DX11Texture                 m_positionArray;

			DX11DepthBuffer             m_depth;

			int                         m_resolution;
			bool                        m_varianceShadowMapping;

			int                         m_csmSplits;
			mye::math::Real             m_csmLogWeight;

			DX11VertexShaderPointer     m_rsmVS;
			DX11PixelShaderPointer      m_singlePS;

			DX11GeometryShaderPointer   m_pssmGS;
			DX11VertexShaderPointer     m_pssmVS;

			std::vector<PSSMSlice>      m_pssmSlices;

			mye::math::Matrix4          m_lightSpaceTransform;
			mye::math::Matrix4          m_cropMatrix[__MYE_RSM_MAX_CSM_COUNT];

			mye::core::Light          * m_light;

			DX11ConstantBuffer          m_transformCBuffer;
			DX11ConstantBuffer          m_cropMatrixCBuffer;
			DX11ConstantBuffer          m_materialCBuffer;
			DX11ConstantBuffer          m_lightCBuffer;

			/* Private functions */

			bool                       __CreateRenderTargets(void);
			void                       __DestroyRenderTargets(void);

			bool                       __CreateConstantBuffers(void);
			void                       __DestroyConstantBuffers(void);

			bool                       __CreateDepthBuffers(void);
			void                       __DestroyDepthBuffers(void);

			void                       __RenderDirectionalLight(mye::core::Light * light);

			std::vector<PSSMSlice>     __CSMComputeSplitsDepths(mye::math::Real nearDistance, mye::math::Real farDistance);

			mye::math::Matrix4         __CSMCropMatrix(const mye::math::Matrix4 & shadowViewProjMatrix,
                                                    const mye::math::Frustum & sliceFrustum,
			                                        const mye::math::AABB    & castersAABB,
			                                        const mye::math::AABB    & receiversAABB,
			                                        mye::math::Real            padding);

			mye::core::GameObjectsList __CSMFindPotentialCasters(const mye::math::Vector3 & lightDirection, const mye::math::Frustum & cameraFrustum);
			PSSMSlice                  __CSMFindTightPlanes(mye::core::Camera * camera, const mye::math::AABB & aabb);
									  
		};

	}

}