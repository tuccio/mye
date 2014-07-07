#include <mye/core/AlignedAllocator.h>
#include <mye/math/Math.h>

namespace mye
{

	namespace dx11
	{

		namespace detail
		{

			__MYE_STRUCT_ALIGNED_16(TransformBuffer)
			{
				mye::math::Matrix4f world;
				mye::math::Matrix4f worldView;
				mye::math::Matrix4f worldViewProjection;
			};

			__MYE_STRUCT_ALIGNED_16(MaterialBuffer)
			{

				mye::math::Vector4f diffuse;
				mye::math::Vector4f specular;

				float specularPower;

			};

			__MYE_STRUCT_ALIGNED_16(LightBuffer)
			{

				mye::math::Vector4f color;
				mye::math::Vector4f position;
				mye::math::Vector4f direction;

				float spotAngle;
				float range;

			};

			__MYE_STRUCT_ALIGNED_16(CameraBuffer)
			{
				mye::math::Vector4f position;
			};

		}

	}

}