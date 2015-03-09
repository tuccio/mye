#include "DX11SwapChain.h"
#include "DX11Utils.h"

using namespace mye::dx11;

DX11SwapChain::DX11SwapChain(void) :
	m_swapChain(nullptr)
{

}

DX11SwapChain::DX11SwapChain(DX11SwapChainConfiguration swapChainConfiguration) :
	m_swapChainConfiguration(swapChainConfiguration),
	m_swapChain(nullptr)
{
}


DX11SwapChain::~DX11SwapChain(void)
{
}

bool DX11SwapChain::Create(void)
{

	return
		CreateSwapChain() &&
		CreateBackBufferView();

}

void DX11SwapChain::Destroy(void)
{
	ReleaseCOMOptional(m_backBufferTargetView);
	ReleaseCOMOptional(m_backBufferResourceView);

}

bool DX11SwapChain::Exists(void) const
{
	return m_swapChain != nullptr;
}

bool DX11SwapChain::CreateSwapChain(void)
{

	DXGI_SWAP_CHAIN_DESC swapDesc;

	auto format = GetDXGIFormat(m_swapChainConfiguration.format);

	swapDesc.BufferDesc.RefreshRate = { m_swapChainConfiguration.refresh.GetNumerator(), m_swapChainConfiguration.refresh.GetDenominator() };

	swapDesc.BufferDesc.Width = m_swapChainConfiguration.width;
	swapDesc.BufferDesc.Height = m_swapChainConfiguration.height;

	swapDesc.BufferDesc.Format = format;

	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapDesc.OutputWindow = m_swapChainConfiguration.window->GetHandle();
	swapDesc.Windowed = !m_swapChainConfiguration.fullscreen;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags = 0;

	switch (m_swapChainConfiguration.msaa)
	{

	case MSAA::MSAA_4X:

		{

			UINT msaa4xQuality;

			m_swapChainConfiguration.device->GetDevice()->CheckMultisampleQualityLevels(
				format,
				4,
				&msaa4xQuality);
	
			assert(msaa4xQuality > 0 && "MSAA 4x is not supported");
	
			swapDesc.SampleDesc.Count   = 4;
			swapDesc.SampleDesc.Quality = msaa4xQuality - 1;

		}

		break;

	case MSAA::MSAA_8X:

		{
			
			UINT msaa8xQuality;
			
			m_swapChainConfiguration.device->GetDevice()->CheckMultisampleQualityLevels(
			  format,
			  8,
			  &msaa8xQuality);
			
			assert(msaa8xQuality > 0 && "MSAA 8x is not supported");
			
			swapDesc.SampleDesc.Count   = 8;
			swapDesc.SampleDesc.Quality = msaa8xQuality - 1;

		}

		break;

	default:

		swapDesc.SampleDesc.Count   = 1;
		swapDesc.SampleDesc.Quality = 0;

		break;

	}

	IDXGIDevice  *dxgiDevice  = nullptr;
	IDXGIAdapter *dxgiAdapter = nullptr;
	IDXGIFactory *dxgiFactory = nullptr;

	bool success = (
		!HRTESTFAILED(m_swapChainConfiguration.device->GetDevice()->
			QueryInterface(__uuidof(IDXGIDevice),
			(void**)&dxgiDevice)) &&

		!HRTESTFAILED(dxgiDevice->GetParent(
			__uuidof(IDXGIAdapter),
			((void**)&dxgiAdapter))) &&

		!HRTESTFAILED(dxgiAdapter->GetParent(
			__uuidof(IDXGIFactory),
			((void**)&dxgiFactory))) &&

		!HRTESTFAILED(dxgiFactory->CreateSwapChain(
			m_swapChainConfiguration.device->GetDevice(),
			&swapDesc,
			&m_swapChain))
		);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	return success;

}

bool DX11SwapChain::CreateBackBufferView(void)
{

	ID3D11Texture2D *backBuffer;

	bool success = (
		!HRTESTFAILED(m_swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer))) &&

		!HRTESTFAILED(m_swapChainConfiguration.device->GetDevice()->CreateRenderTargetView(
			backBuffer,
			nullptr,
			&m_backBufferTargetView)) &&


		!HRTESTFAILED(m_swapChainConfiguration.device->GetDevice()->CreateShaderResourceView(
			backBuffer,
			nullptr,
			&m_backBufferResourceView))
		);

	ReleaseCOM(backBuffer);

	return success;

}

void DX11SwapChain::Resize(int width, int height)
{

	ReleaseCOM(m_backBufferResourceView);
	ReleaseCOM(m_backBufferTargetView);

	m_swapChainConfiguration.width  = width;
	m_swapChainConfiguration.height = height;

	m_swapChain->ResizeBuffers(
		1,
		width,
		height,
		GetDXGIFormat(m_swapChainConfiguration.format),
		0);

	CreateBackBufferView();

}

void DX11SwapChain::ClearBackBuffer(const mye::math::Vector4f &color)
{

	m_swapChainConfiguration.device->GetImmediateContext()->ClearRenderTargetView(
		m_backBufferTargetView,
		color.Data());

}