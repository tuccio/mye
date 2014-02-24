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
				const mye::core::String &name,
				mye::core::ManualResourceLoader *manual,
				mye::dx11::DX11Device &device);

			~DX11PixelShader(void);

			void Use(void);

			ID3D11PixelShader* GetPixelShader(void);
			mye::core::String GetCompileError(void);

			void Destroy(void);

		private:

			bool LoadImpl(void);
			void UnloadImpl(void);
			size_t CalculateSizeImpl(void);

			ID3D11PixelShader *m_shader;
			mye::core::String m_compileError;

		private:

			DX11Device &m_device;

		};

	}

}
