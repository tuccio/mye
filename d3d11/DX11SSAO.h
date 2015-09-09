#pragma once

#include "DX11ShaderProgram.h"
#include "DX11Texture.h"

namespace mye
{

	namespace dx11
	{

		class DX11SSAO
		{
			
		public:

			DX11SSAO(void);

			bool Create(int width, int height);
			void Destroy(void);

			void Render(void);

			DX11ShaderResource & GetSSAOBuffer(void);

			int GetWidth(void) const;
			int GetHeight(void) const;

		private:

			int m_width;
			int m_height;

			DX11Texture m_blurBuffer;
			DX11Texture m_ssaoBuffer;

			DX11ShaderProgramPointer m_ssao;
			DX11ShaderProgramPointer m_ssaoBlur;

		};

	}

}