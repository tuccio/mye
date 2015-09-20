#pragma once

#include "DX11Shader.h"

namespace mye
{

	namespace dx11
	{

		class DX11ComputeShader :
			public DX11Shader
		{

		public:

			DX11ComputeShader(mye::core::ResourceManager * owner,
							  const mye::core::String & name,
							  mye::core::ManualResourceLoader * manual,
							  bool precompiled = false);

			~DX11ComputeShader(void);

			void Use(void);
			void Dispose(void);

			void Destroy(void);

			ID3D11ComputeShader     * GetComputeShader(void);
			const mye::core::String & GetCompileError(void);

		protected:

			bool   LoadImpl(void);
			void   UnloadImpl(void);
			size_t CalculateSizeImpl(void);

			ID3D11ComputeShader * m_shader;
			mye::core::String     m_compileError;


		};

		typedef std::shared_ptr<DX11ComputeShader> DX11ComputeShaderPointer;

	}
}
