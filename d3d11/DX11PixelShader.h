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

			DX11PixelShader(mye::core::ResourceManager      * owner,
							const mye::core::String         & name,
							mye::core::ManualResourceLoader * manual,
							bool precompiled = false);

			~DX11PixelShader(void);

			void Use(void);
			void Dispose(void);

			void Destroy(void);

			ID3D11PixelShader       * GetPixelShader(void);
			const mye::core::String & GetCompileError(void);

		protected:

			bool   LoadImpl(void);
			void   UnloadImpl(void);
			size_t CalculateSizeImpl(void);

		private:

			ID3D11PixelShader * m_shader;
			mye::core::String   m_compileError;

		};

		typedef std::shared_ptr<DX11PixelShader> DX11PixelShaderPointer;

	}

}
