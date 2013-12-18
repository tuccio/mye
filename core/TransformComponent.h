#pragma once

#include "Component.h"

#include <mye/math/Vector.h>
#include <mye/math/Transform.h>

namespace mye
{

	namespace core
	{

		struct TransformComponent :
			public Component,
			public mye::math::Transform
		{

		public:

			static const char* Name(void);

			TransformComponent(void);
			~TransformComponent(void);

			Cloneable* Clone(void) const;

			Component& operator= (const Component& component);

		};

	}
}
