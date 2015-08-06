#pragma once

#include "DX11Shader.h"

namespace mye
{

	namespace dx11
	{

		class DX11VertexShader :
			public DX11Shader
		{

		public:

			DX11VertexShader(mye::core::ResourceManager * owner,
							 const mye::core::String & name,
							 mye::core::ManualResourceLoader * manual,
							 bool precompiled = false);

			~DX11VertexShader(void);

			void Use(void);
			void Dispose(void);

			void Destroy(void);

			ID3D11VertexShader * GetVertexShader(void);
			mye::core::String    GetCompileError(void);

		protected:

			bool   LoadImpl(void);
			void   UnloadImpl(void);
			size_t CalculateSizeImpl(void);

			ID3D11VertexShader * m_shader;
			ID3D11InputLayout  * m_inputLayout;
			mye::core::String    m_compileError;


		};

		typedef std::shared_ptr<DX11VertexShader> DX11VertexShaderPointer;

	}
}
