#pragma once

#include "Camera.h"
#include "Component.h"

namespace mye
{

	namespace core
	{

		class CameraComponent :
			public Component,
			public Camera
		{

		public:

			CameraComponent(void);
			~CameraComponent(void);

			ComponentTypes GetComponentType(void) const;

			CameraComponent* Clone(void) const;

		};

	}

}
