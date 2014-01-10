#pragma once

#include "DX11Device.h"
#include <d3d11.h>
#include <Effects11\d3dx11effect.h>
#include <string>

namespace mye
{

	namespace dx11
	{

		class DX11Shader
		{

		public:

			enum Type
			{
				SHADERTYPE_VERTEX,
				SHADERTYPE_PIXEL,
				SHADERTYPE_EFFECTS
			};

			DX11Shader(DX11Device &device);
			~DX11Shader(void);

			bool Compile(const std::string &src,
				Type type,
				std::string &infoLog = std::string());

		private:

			union
			{
				ID3D11VertexShader *vertex;
				ID3D11PixelShader *pixel;
				ID3DX11Effect *effect;
			} m_shader;

			DX11Device &m_device;

		};

	}

}
