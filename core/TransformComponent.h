#pragma once

#include "VariableComponent.h"
#include <mye/math/Math.h>

namespace mye
{

	namespace core
	{
		class TransformComponent :
			public VariableComponent<mye::math::Transformf>
		{

		public:

			TransformComponent(void);
			~TransformComponent(void);

		};
	}
}

