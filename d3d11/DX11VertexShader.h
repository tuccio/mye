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

			DX11VertexShader(mye::core::ResourceManager *owner,
				const std::string &name,
				mye::core::ManualResourceLoader *manual,
				mye::dx11::DX11Device &device);

			~DX11VertexShader(void);

			void Use(void);

			ID3D11VertexShader* GetVertexShader(void);
			std::string GetCompileError(void);

			void Destroy(void);

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);
			size_t CalculateSizeImpl(void);

			ID3D11VertexShader *m_shader;
			ID3D11InputLayout *m_inputLayout;
			std::string m_compileError;
			DX11Device &m_device;
			

		};

	}
}
