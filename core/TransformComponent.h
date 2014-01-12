#pragma once

#include "Transform.h"
#include "VariableComponent.h"

namespace mye
{

	namespace core
	{
		class TransformComponent :
			public VariableComponent<Transform>
		{

		public:

			TransformComponent(void);
			~TransformComponent(void);

		};
	}
}

