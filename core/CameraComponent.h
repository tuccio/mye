#pragma once

#include "Camera.h"
#include "Component.h"
#include "AlignedAllocator.h"

namespace mye
{

	namespace core
	{

		class CameraComponent :
			public Component,
			public Camera
		{

		public:

			__MYE_DECLARE_ALIGNED_16

			CameraComponent(void);
			~CameraComponent(void);

			ComponentTypes GetComponentType(void) const;

			CameraComponent * Clone(void) const;

		};

	}

}
