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

			DX11BufferManager(DX11Device &device);
			~DX11BufferManager(void);

		protected:

			mye::core::ResourceHandle CreateImpl(const std::string &name,
				mye::core::ManualResourceLoader *manual,
				mye::core::Resource::ParametersList *params);

			void FreeImpl(mye::core::Resource* resource);

		private:

			DX11Device &m_device;

		};

	}

}
