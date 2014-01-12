#pragma once

#include <mye/core/Singleton.h>
#include <mye/core/ResourceManager.h>
#include <mye/d3d11/DX11Shader.h>

#include <unordered_map>
#include <string>

namespace mye
{

	namespace dx11
	{

		class DX11ShaderManager :
			public mye::core::ResourceManager
		{

		public:

			DX11ShaderManager(DX11Device &device);
			~DX11ShaderManager(void);

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
