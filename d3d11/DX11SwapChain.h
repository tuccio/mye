#pragma once

#include <mye/math/Math.h>
#include <mye/core/VertexDeclaration.h>

#include <mye/win/Window.h>

#include "DX11Device.h"

namespace mye
{

	namespace dx11
	{

		struct DX11SwapChainConfiguration
		{

			mye::win::Window                  * window;
							                 
			int                                 width;
			int                                 height;
			bool                                fullscreen;
								                
			mye::core::DataFormat               format;
			mye::math::Rational<unsigned int>   refresh;
			MSAA                                msaa;

		};

		class DX11SwapChain
		{

		public:

			DX11SwapChain(void);
			DX11SwapChain(DX11SwapChainConfiguration swapChainConfiguration);
			~DX11SwapChain(void);

			bool Create(void);
			void Destroy(void);

			bool Exists(void) const;

			void Resize(int width, int height);

			inline IDXGISwapChain * operator-> (void);

			inline IDXGISwapChain * GetSwapChain(void);
			
			inline ID3D11RenderTargetView   * GetBackBufferRenderTargetView(void);
			inline ID3D11ShaderResourceView * GetBackBufferShaderResourceView(void);

			void ClearBackBuffer(const mye::math::Vector4f & color);

		private:

			bool CreateSwapChain(void);
			bool CreateBackBufferView(void);

			DX11SwapChainConfiguration   m_swapChainConfiguration;
									   
			IDXGISwapChain             * m_swapChain;
									   
			ID3D11RenderTargetView     * m_backBufferTargetView;
			ID3D11ShaderResourceView   * m_backBufferResourceView;

		};

		IDXGISwapChain * DX11SwapChain::operator-> (void)
		{
			return m_swapChain;
		}

		IDXGISwapChain * DX11SwapChain::GetSwapChain(void)
		{
			return m_swapChain;
		}

		ID3D11RenderTargetView * DX11SwapChain::GetBackBufferRenderTargetView(void)
		{
			return m_backBufferTargetView;
		}

		ID3D11ShaderResourceView * DX11SwapChain::GetBackBufferShaderResourceView(void)
		{
			return m_backBufferResourceView;
		}

	}

}
