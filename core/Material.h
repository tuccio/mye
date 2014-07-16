#pragma once

#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{

		struct Material
		{

			mye::math::Vector4 color;

			mye::math::Real specular;
			mye::math::Real roughness;
			mye::math::Real metallic;

		};

	}
}
