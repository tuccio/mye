#pragma once

#include "Component.h"

namespace mye
{

	namespace core
	{

		struct TransformComponent :
			public Component
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
