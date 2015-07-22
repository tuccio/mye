#include <mye/core/Singleton.h>
#include <mye/core/ResourceManager.h>

namespace mye
{

	namespace core
	{

		class GPUBufferManager :
			public mye::core::ResourceManager,
			public mye::core::Singleton<GPUBufferManager> { };

	}

}