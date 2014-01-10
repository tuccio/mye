#pragma once

#include <mye/win/Window.h>

#include "DX11Device.h"

namespace mye
{

	namespace dx11
	{

		class DX11Window :
			public mye::win::Window
		{

		public:

			enum MSAA
			{
				MSAA_OFF,
				MSAA_4x,
				MSAA_8x,
			};

			DX11Window(DX11Device &device);
			~DX11Window(void);

			bool Init(void);
			void Clear(void);

			bool CreateSwapChain(void);
			bool CreateRenderTargetView(void);
			bool CreateDepthStencilBuffers(void);

			void SetRefreshRate(DXGI_RATIONAL refresh);
			void SetMSAA(MSAA msaa);

			void ResizeBuffers(int width, int height);
			void SetViewport(int x, int y, int width, int height);

		private:

			DX11Device &m_device;

			DXGI_RATIONAL m_refresh;
			IDXGISwapChain *m_swapChain;

			ID3D11Texture2D *m_depthStencilBuffer;

			ID3D11RenderTargetView *m_renderTargetView;
			ID3D11DepthStencilView *m_depthStencilView;

			MSAA m_msaa;

			class ResizeListener :
				public IWindow::Listener
			{
			public:
				void OnResize(IWindow *window, const Eigen::Vector2i &size);
			} m_resizeListener;

		};

	}

}