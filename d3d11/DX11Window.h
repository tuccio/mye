#pragma once

#include <mye/win/Window.h>
#include <mye/math/Math.h>

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
			void ShutDown(void);

			DX11Device& GetDevice(void);

			bool CreateSwapChain(void);
			bool CreateRenderTargetView(void);
			bool CreateDepthStencilBuffers(void);

			void SetRefreshRate(DXGI_RATIONAL refresh);
			void SetMSAA(MSAA msaa);

			void ClearColorBuffer(const mye::math::Vector4f &color = mye::math::Vector4f (0, 0, 0, 1));
			void ClearDepthBuffer(float depth = 1.0f);

			void ResizeBuffers(int width, int height);
			void SetViewport(int x, int y, int width, int height);

			inline ID3D11RenderTargetView* GetRenderTargetView(void);
			inline ID3D11DepthStencilView* GetDepthStencilView(void);
			inline IDXGISwapChain* GetSwapChain(void);

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
				void OnResize(IWindow *window, const mye::math::Vector2i &size);
			} m_resizeListener;

		};

		ID3D11RenderTargetView* DX11Window::GetRenderTargetView(void)
		{
			return m_renderTargetView;
		}

		ID3D11DepthStencilView* DX11Window::GetDepthStencilView(void)
		{
			return m_depthStencilView;
		}

		IDXGISwapChain* DX11Window::GetSwapChain(void)
		{
			return m_swapChain;
		}

	}

}
