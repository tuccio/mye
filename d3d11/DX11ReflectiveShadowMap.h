#include "DX11DepthBuffer.h"
#include "DX11Texture.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

#include <mye/core/Light.h>
#include <mye/math/Math.h>

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

			void Bind(DX11PipelineStage stage);
			void Unbind(void);

			inline int GetResolution(void) const
			{
				return m_resolution;
			}

			void SetResolution(int resolution);

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

			inline DX11ShaderResource & GetVSMShaderResource(void)
			{
				return m_vsm;
			}

			inline const mye::math::Matrix4 & GetLightSpaceTransformMatrix(void)
			{
				return m_lightSpaceTransform;
			}

		private:

			DX11Device      & m_device;

			bool              m_initialized;
						  
			DX11Texture       m_position;
			DX11Texture       m_normal;
			DX11Texture       m_flux;
			DX11Texture       m_vsm;
						     
			DX11DepthBuffer   m_depth;

			int  m_resolution;
			bool m_varianceShadowMapping;

			DX11VertexShaderPointer m_rsmVS;
			DX11PixelShaderPointer  m_rsmPS;
			DX11PixelShaderPointer  m_rsmVSMPS;

			mye::math::Matrix4 m_lightSpaceTransform;

			bool CreateRenderTargets(void);

			void RenderDirectional(mye::core::Light * light);

		};

	}

}

namespace mye
{

	namespace math
	{


		template <typename T = mye::math::Real>
		Matrix<T, 4, 4> OrthographicProjection(T w, T h, T n, T f)
		{

			/*T iWidth  = T(1) / w;
			T iHeight = T(1) / h;
			T iDepth  = T(1) / (f - n);

			Matrix<T, 4, 4> m(1);

			m.m00() = T(2) * iWidth;
			m.m11() = T(2) * iHeight;
			m.m22() = T(1) * iDepth;

			m.m23() = - n * iDepth;
*/

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
		Matrix<T, 4, 4> OrthographicProjection(T l, T r, T b, T t, T n, T f)
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