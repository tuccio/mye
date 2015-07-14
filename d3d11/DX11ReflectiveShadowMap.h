#include "DX11DepthBuffer.h"
#include "DX11Texture.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

#include <mye/core/Camera.h>
#include <mye/core/Light.h>
#include <mye/math/Math.h>

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

			DX11ReflectiveShadowMap(DX11Device & device, bool vsm);

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

			inline int GetCSMSlices(void) const
			{
				return m_csmSlices;
			}

			inline void SetCSMSlices(int slices)
			{
				m_csmSlices = slices;
			}

			inline float GetCSMLogarithmicWeight(void) const
			{
				return m_csmLogWeight;
			}

			inline void SetCSMLogarithmicWeight(float weight)
			{
				m_csmLogWeight = weight;
			}

			inline DX11ShaderResource & GetPositionShaderResource(int slice = 0)
			{
				return m_position[slice];
			}

			inline DX11ShaderResource & GetNormalShaderResource(int slice = 0)
			{
				return m_normal[slice];
			}

			inline DX11ShaderResource & GetFluxShaderResource(int slice = 0)
			{
				return m_flux[slice];
			}

			inline DX11ShaderResource & GetDepthShaderResource(int slice = 0)
			{
				return m_depth[slice];
			}

			inline DX11ShaderResource & GetVSMShaderResource(int slice = 0)
			{
				return m_vsm[slice];
			}

			inline const mye::math::Matrix4 & GetLightSpaceTransformMatrix(int slice = 0)
			{
				return m_lightSpaceTransform[slice];
			}

		private:

			DX11Device & m_device;

			bool m_initialized;
			int  m_lastBoundSlice;

			std::vector<DX11Texture> m_position;
			std::vector<DX11Texture> m_normal;
			std::vector<DX11Texture> m_flux;
			std::vector<DX11Texture> m_vsm;

			DX11Texture m_positionArray;

			std::vector<DX11DepthBuffer> m_depth;

			int  m_resolution;
			bool m_varianceShadowMapping;

			int             m_csmSlices;
			mye::math::Real m_csmLogWeight;

			DX11VertexShaderPointer m_rsmVS;
			DX11PixelShaderPointer  m_rsmPS;
			DX11PixelShaderPointer  m_rsmVSMPS;

			mye::math::Matrix4 m_lightSpaceTransform[__MYE_RSM_MAX_CSM_COUNT];

			bool __CreateRenderTargets(void);
			void __DestroyRenderTargets(void);

			bool __CreateDepthBuffers(void);
			void __DestroyDepthBuffers(void);

			struct __CSMSlices
			{
				mye::math::Real near;
				mye::math::Real far;
			};

			void                     __RenderDirectionalLight(mye::core::Light * light);
			std::vector<__CSMSlices> __CSMComputeSplitsDepths(mye::math::Real nearDistance, mye::math::Real farDistance);
			mye::math::Matrix4       __CSMCropMatrix(const mye::math::Matrix4 & shadowViewMatrix,
                                                     const mye::math::Matrix4 & shadowProjMatrix,
                                                     const mye::math::Frustum & sliceFrustum);

		};

	}

}

namespace mye
{

	namespace math
	{


		template <typename T = mye::math::Real>
		Matrix<T, 4, 4> OrthographicProjectionLH(T w, T h, T n, T f)
		{

			T iWidth  = T(1) / w;
			T iHeight = T(1) / h;
			T iDepth  = T(1) / (f - n);

			Matrix<T, 4, 4> m(1);

			m.m00() = T(2) * iWidth;
			m.m11() = T(2) * iHeight;
			m.m22() = T(1) * iDepth;

			m.m23() = -n * iDepth;

			return m;

		}

		template <typename T = mye::math::Real>
		Matrix<T, 4, 4> OrthographicProjectionLH(T l, T r, T b, T t, T n, T f)
		{

			T iRmL = T(1) / (r - l);
			T iTmB = T(1) / (t - b);
			T iFmN = T(1) / (f - n);

			Matrix<T, 4, 4> m(1);

			m.m00() = T(2) * iRmL;
			m.m03() = - (l + r) * iRmL;

			m.m11() = T(2) * iTmB;
			m.m13() = - (t + b) * iTmB;

			m.m22() = T(1) * iFmN;
			m.m23() = - n * iFmN;

			return m;

		}

	}

}