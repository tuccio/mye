#pragma once

#include <mye/core/Singleton.h>
#include <mye/core/ResourceManager.h>

#include "DX11Buffer.h"

#include <unordered_map>
#include <string>

namespace mye
{

	namespace dx11
	{

		class DX11BufferManager :
			public mye::core::ResourceManager,
			public mye::core::Singleton<DX11BufferManager>
		{

		public:

			DX11BufferManager(void);
			~DX11BufferManager(void);

		protected:

			mye::core::Resource * CreateImpl(const mye::core::String & name,
			                                 mye::core::ManualResourceLoader * manual,
			                                 const mye::core::Parameters & params);

			void FreeImpl(mye::core::Resource * resource);

		};

	}

}
