#pragma once

#include "DX11Shader.h"

namespace mye
{

	namespace dx11
	{

		class DX11PixelShader :
			public DX11Shader
		{

		public:

			DX11PixelShader(mye::core::ResourceManager *owner,
				const std::string &name,
				mye::core::ManualResourceLoader *manual,
				mye::dx11::DX11Device &device);

			~DX11PixelShader(void);

			ID3D11PixelShader* GetPixelShader(void);
			std::string GetCompileError(void);

		private:

			bool LoadImpl(void);
			void UnloadImpl(void);
			size_t CalculateSizeImpl(void);

			ID3D11PixelShader *m_shader;
			std::string m_compileError;

		private:

			DX11Device &m_device;

		};

	}

}
