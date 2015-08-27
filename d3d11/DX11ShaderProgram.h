#pragma once

#include <mye/core/Resource.h>

#include "DX11VertexShader.h"
#include "DX11GeometryShader.h"
#include "DX11PixelShader.h"
#include "DX11Shader.h"

namespace mye
{

	namespace dx11
	{

		class DX11ShaderProgram :
			public DX11Shader
		{

		public:

			DX11ShaderProgram(mye::core::ResourceManager * owner,
			                  const mye::core::String & name,
			                  mye::core::ManualResourceLoader * manual);

			void Use(void);
			void Dispose(void);

			void Destroy(void);

		private:

			DX11VertexShaderPointer   m_vertexShader;
			DX11GeometryShaderPointer m_geometryShader;
			DX11PixelShaderPointer    m_pixelShader;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			size_t CalculateSizeImpl(void);

		};

		typedef std::shared_ptr<DX11ShaderProgram> DX11ShaderProgramPointer;

	}

}