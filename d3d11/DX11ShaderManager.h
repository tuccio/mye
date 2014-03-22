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
			public mye::core::ResourceManager,
			public mye::core::Singleton<DX11ShaderManager>
		{

		public:

			DX11ShaderManager(DX11Device &device);
			~DX11ShaderManager(void);

			DX11ShaderPointer CreateVertexShader(const mye::core::String &name,
				bool precompiled,
				const std::vector<D3D11_INPUT_ELEMENT_DESC> &vdesc);

			DX11ShaderPointer CreatePixelShader(const mye::core::String &name,
				bool precompiled);

		protected:

			DX11Shader* CreateImpl(const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				const mye::core::Resource::ParametersList &params);

			void FreeImpl(mye::core::Resource* resource);

		private:

			DX11Device &m_device;

		};

	}

}
