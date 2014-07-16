//#include <mye/core/AlignedAllocator.h>
#include <mye/math/Math.h>

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

				mye::math::Vector4f color;

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

				float spotAngle;
				float range;

				float __fill[2];

			};

			struct CameraBuffer
			{
				mye::math::Vector4f position;
			};

		}

	}

}