//#include <mye/core/AlignedAllocator.h>
#include <mye/math/Math.h>

#include "../DX11ConstantBuffer.h"

#include <tuple>

#undef near
#undef far

namespace mye
{

	namespace dx11
	{

		namespace detail
		{

			template <typename T, int N>
			inline void VectorCopy(void * dest, const mye::math::Matrix<T, N, 1> & src)
			{
				std::memcpy(dest, src.Data(), sizeof(T) * N);
			}

			struct TransformBuffer
			{

				mye::math::Matrix4 world;
				mye::math::Matrix4 worldView;
				mye::math::Matrix4 worldViewProjection;

			};

			struct MaterialBuffer
			{

				float diffuseColor[3];
				float __fill0;

				float specularColor[3];
				float specular;

				float roughness;
				float metallic;
				float __fill2[2];

			};

			struct LightBuffer
			{

				float color[3];
				float __fill0;

				float position[3];
				float __fill1;

				float direction[3];
				float intensity;

				float spotAngle;
				float range;

				float __fill[2];

			};

			struct __MYE_ALIGNED(16) RenderConfigurationBuffer
			{

				int          resolution[2];

				float        gamma;

				int          shadowMapResolution;
				float        shadowMapInvResolution;
				float        shadowMapBias;
				float        shadowMapSlopeScaledBias;
				float        shadowMapNormalOffsetBias;

				float        vsmMinVariance;
				float        vsmMinBleeding;

				float        esmPositiveExponent;
				float        esmNegativeExponent;

				unsigned int csmSplits;
				int          csmDebug;
				
				int          pcfEnabled;
				unsigned int pcfKernel;
				float        pcfKernelInvSquare;

				float        lpvAttenuation;

				float        ppBloomThreshold;
				float        ppBloomPower;

			};

			struct PSSMSlice
			{

				mye::math::Matrix4 cropMatrix;

				float              near;
				float              far;

				float              __fill[2];

			};

			struct LPVConfiguration
			{

				float        minCorner[3];
				float        __fill0;

				float        maxCorner[3];					        
				float        cellSize;

				unsigned int rsmResolution;
				unsigned int lpvResolution;

				float        geometryInjectionBias;
				float        fluxInjectionBias;

			};

			struct CameraBuffer
			{

				mye::math::Matrix4 view;
				mye::math::Matrix4 viewProj;
				mye::math::Matrix4 invViewProj;

				float              position[3];

				float              near;
				float              far;
				float              fovy;
				float              ratio;

				float              __fill;

			};

		}

		inline void MakeTransformBuffer(DX11ConstantBuffer & b,
		                                const mye::math::Matrix4 & world,
		                                const mye::math::Matrix4 & view,
		                                const mye::math::Matrix4 & proj)
		{

			detail::TransformBuffer t;

			t.world               = world;
			t.worldView           = view * world;
			t.worldViewProjection = proj * t.worldView;

			b.SetData(&t);

		}

		template <typename M>
		inline void MakeMaterialBuffer(DX11ConstantBuffer & b,
									   M material)
		{

			detail::MaterialBuffer materialBuffer;

			detail::VectorCopy(materialBuffer.diffuseColor, material->GetDiffuseColor());
			detail::VectorCopy(materialBuffer.specularColor, material->GetSpecularColor());

			materialBuffer.specular      = material->GetSpecular();
			materialBuffer.metallic      = material->GetMetallic();
			materialBuffer.roughness     = material->GetRoughness();

			b.SetData(&materialBuffer);

		}

		template <typename L>
		inline void MakeLightBuffer(DX11ConstantBuffer & b, L light)
		{

			const mye::math::Matrix4 & t = light->GetWorldMatrix();

			detail::LightBuffer lightBuffer;

			mye::math::Vector3 position  = (t * mye::math::Vector4(light->GetPosition(), 1)).xyz();
			mye::math::Vector3 direction = (t * mye::math::Vector4(light->GetDirection(), 1)).xyz();

			detail::VectorCopy(lightBuffer.color,     light->GetColor());
			detail::VectorCopy(lightBuffer.position,  position);
			detail::VectorCopy(lightBuffer.direction, direction.Normalize());

			lightBuffer.spotAngle = light->GetSpotAngle();
			lightBuffer.range     = light->GetRange();
			lightBuffer.intensity = light->GetIntensity();

			b.SetData(&lightBuffer);

		}

		inline void MakeLPVConfigurationBuffer(DX11ConstantBuffer & b,
		                                       const mye::math::Vector3 & minCorner,
											   const mye::math::Vector3 & maxCorner,
											   mye::math::Real cellSize,
											   unsigned int rsmResolution,
											   unsigned int lpvResolution,
											   mye::math::Real geometryInjectionBias,
											   mye::math::Real fluxInjectionBias)
		{

			detail::LPVConfiguration lpvConfig;

			detail::VectorCopy(lpvConfig.minCorner, minCorner);
			detail::VectorCopy(lpvConfig.maxCorner, maxCorner);

			lpvConfig.cellSize      = cellSize;
			lpvConfig.rsmResolution = rsmResolution;
			lpvConfig.lpvResolution = lpvResolution;

			lpvConfig.geometryInjectionBias = geometryInjectionBias;
			lpvConfig.fluxInjectionBias     = fluxInjectionBias;

			b.SetData(&lpvConfig);

		}

		inline void MakeCameraBuffer(DX11ConstantBuffer & b,
		                             const mye::math::Matrix4 & view,
		                             const mye::math::Matrix4 & viewProj,
		                             const mye::math::Matrix4 & invViewProj,
									 const mye::math::Vector3 & position,
		                             float near, float far,
		                             float fovy, float ratio)
		{

			detail::CameraBuffer cb;

			cb.view        = view;
			cb.viewProj    = viewProj;
			cb.invViewProj = invViewProj;
			cb.near        = near;
			cb.far         = far;
			cb.fovy        = fovy;
			cb.ratio       = ratio;

			detail::VectorCopy(cb.position, position);

			b.SetData(&cb);

		}

		template <typename ... Args>
		inline void MakeConstantBuffer(DX11ConstantBuffer & b, Args && ... args)
		{
			auto t = std::make_tuple(args ...);
			b.SetData(&t);
		}

	}

}