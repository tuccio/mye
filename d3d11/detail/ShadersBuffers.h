//#include <mye/core/AlignedAllocator.h>
#include <mye/math/Math.h>

#undef near
#undef far

namespace mye
{

	namespace dx11
	{

		namespace detail
		{

			struct TransformBuffer
			{

				mye::math::Matrix4f world;
				mye::math::Matrix4f worldView;
				mye::math::Matrix4f worldViewProjection;

			};

			struct MaterialBuffer
			{

				mye::math::Vector4f diffuseColor;
				mye::math::Vector4f specularColor;

				float specular;
				float roughness;
				float metallic;

				float __fill[1];

			};

			struct LightBuffer
			{

				mye::math::Vector4f color;
				mye::math::Vector4f position;
				mye::math::Vector4f direction;

				float intensity;

				float spotAngle;
				float range;

				float __fill[1];

			};

			struct CameraBuffer
			{
				mye::math::Vector4f position;
			};

			struct __MYE_ALIGNED(16) RenderConfigurationBuffer
			{

				float        resolution[2];

				float        gamma;

				int          shadowMapResolution;
				float        shadowMapInvResolution;
				float        shadowMapBias;
				float        shadowMapSlopeScaledBias;
				float        shadowMapNormalOffsetBias;

				float        vsmMinVariance;
				float        vsmMinBleeding;

				unsigned int csmSplits;
				int          csmDebug;
				
				int          pcfEnabled;
				unsigned int pcfKernel;
				float        pcfKernelInvSquare;

			};

			struct PSSMSlice
			{
				mye::math::Matrix4 cropMatrix;
				float              near;
				float              far;
				float              __fill[2];
			};
		}

	}

}