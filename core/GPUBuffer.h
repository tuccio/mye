#pragma once

#include "Resource.h"

namespace mye
{

	namespace core
	{

		class GPUBuffer :
			public mye::core::Resource
		{

		public:

			GPUBuffer(ResourceManager *owner,
				const String &name,
				ManualResourceLoader *manual);

			~GPUBuffer(void);

		};

		typedef std::shared_ptr<GPUBuffer> GPUBufferPointer;

	}

}
