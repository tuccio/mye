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
				mye::dx11::DX11Device &device,
				bool precompiled = false);

			~DX11PixelShader(void);

			void Use(void);
			void Destroy(void);

			ID3D11PixelShader*       GetPixelShader(void);
			const mye::core::String& GetCompileError(void);

		protected:

			bool   LoadImpl(void);
			void   UnloadImpl(void);
			size_t CalculateSizeImpl(void);

		private:

			ID3D11PixelShader *m_shader;
			mye::core::String  m_compileError;

			DX11Device        &m_device;

		};

		typedef std::shared_ptr<DX11PixelShader> DX11PixelShaderPointer;

	}

}
