#pragma once

#include "Light.h"
#include "Component.h"

namespace mye
{

	namespace core
	{

		class LightComponent :
			public Component,
			public Light
		{

		public:

			LightComponent(void);
			LightComponent(const Light &light);

			~LightComponent(void);

			LightComponent* Clone(void) const;

		};

	}

}
