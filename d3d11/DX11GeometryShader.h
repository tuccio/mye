#pragma once

#include "DX11Shader.h"

namespace mye
{

	namespace dx11
	{

		class DX11GeometryShader :
			public DX11Shader
		{

		public:

			DX11GeometryShader(mye::core::ResourceManager * owner,
			                   const mye::core::String & name,
			                   mye::core::ManualResourceLoader * manual,
			                   bool precompiled = false);

			~DX11GeometryShader(void);

			void Use(void);
			void Dispose(void);

			void Destroy(void);

			ID3D11GeometryShader * GetGeometryShader(void);
			mye::core::String      GetCompileError(void);

		protected:

			bool   LoadImpl(void);
			void   UnloadImpl(void);
			size_t CalculateSizeImpl(void);

			ID3D11GeometryShader * m_shader;
			mye::core::String      m_compileError;


		};

		typedef std::shared_ptr<DX11GeometryShader> DX11GeometryShaderPointer;

	}
}
