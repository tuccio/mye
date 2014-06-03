#pragma once

#include <mye/core/GraphicsModule.h>

#include "DX11Device.h"
#include "DX11Window.h"
#include "DX11ConstantBuffer.h"
#include "DX11VertexShader.h"
#include "DX11PixelShader.h"

namespace mye
{

	namespace dx11
	{

		class DX11Module :
			public mye::core::GraphicsModule
		{

		public:

			DX11Module(void);
			DX11Module(DX11Window *window);

			~DX11Module(void);

			bool Init(void);
			void ShutDown(void);

			void Render(void);

			void NewWindow(void);
			void SetWindow(DX11Window *window);
			void FreeWindow(void);

			mye::math::Vector2f GetPointSize(void);

		private:

			DX11Device *m_device;
			DX11Window *m_window;
			bool m_ownedDevice;

			DX11ConstantBuffer *m_mvpBuffer;
			DX11ConstantBuffer *m_textColorBuffer;

			// Text 2D shaders

			DX11VertexShaderPointer m_text2dvs;
			DX11PixelShaderPointer  m_text2dps;

			// 3D Shaders

			DX11VertexShaderPointer m_basicvs;
			DX11PixelShaderPointer  m_basicps;

			// Samplers

			ID3D11SamplerState     *m_modelTextureSampler;
			ID3D11SamplerState     *m_fontTextureSampler;

		};

	}

}
