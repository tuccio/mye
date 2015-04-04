#pragma once

#include "DX11Device.h"

#include "DX11ConstantBuffer.h"
#include "DX11DepthBuffer.h"
#include "DX11SwapChain.h"
#include "DX11Texture.h"

#include "DX11PixelShader.h"
#include "DX11VertexShader.h"

#include <mye/win/Window.h>

namespace mye
{

	namespace dx11
	{

		class DX11BasicRenderer :
			public mye::core::IWindow::Listener
		{

		public:

			DX11BasicRenderer(DX11Device &device, mye::win::Window *window);
			~DX11BasicRenderer(void);

			bool Init(void);
			void Shutdown(void);

			void Render(ID3D11RenderTargetView *target);

			void SetWindow(mye::win::Window *window);
			void OnResize(mye::core::IWindow *window, const mye::math::Vector2i &size);

		private:

			bool                    m_initialized;

			DX11Device             &m_device;

			mye::win::Window       *m_window;

			DX11VertexShaderPointer m_basicVS;
			DX11PixelShaderPointer  m_basicPS;

			DX11ConstantBuffer      m_transformBuffer;

			DX11DepthBuffer         m_depthBuffer;

			mye::math::Vector4f     m_clearColor;

			bool CreateConstantBuffers(void);

		};

	}

}
