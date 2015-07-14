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
			CameraComponent(const Camera & camera);
			~CameraComponent(void);

			ComponentTypes GetComponentType(void) const;

			Camera GetCamera(void) const;
			void SetCamera(const Camera & camera);

			CameraComponent * Clone(void) const;

		};

	}

}
