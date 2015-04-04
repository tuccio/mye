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
		CreateSwapChain() && CreateBackBufferView();

}

void DX11SwapChain::Destroy(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_backBufferTargetView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_backBufferResourceView);

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

	swapDesc.BufferDesc.Width  = m_swapChainConfiguration.width;
	swapDesc.BufferDesc.Height = m_swapChainConfiguration.height;

	swapDesc.BufferDesc.Format = format;

	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	swapDesc.BufferCount  = 1;
	swapDesc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapDesc.OutputWindow = m_swapChainConfiguration.window->GetHandle();
	swapDesc.Windowed     = !m_swapChainConfiguration.fullscreen;
	swapDesc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags        = 0;

	swapDesc.SampleDesc = m_swapChainConfiguration.device->GetMSAASampleDesc(m_swapChainConfiguration.msaa, GetDXGIFormat(m_swapChainConfiguration.format));

	IDXGIDevice  * dxgiDevice  = nullptr;
	IDXGIAdapter * dxgiAdapter = nullptr;
	IDXGIFactory * dxgiFactory = nullptr;

	bool success = (
		!__MYE_DX11_HR_TEST_FAILED(m_swapChainConfiguration.device->GetDevice()->
			QueryInterface(__uuidof(IDXGIDevice),
			(void**)&dxgiDevice)) &&

		!__MYE_DX11_HR_TEST_FAILED(dxgiDevice->GetParent(
			__uuidof(IDXGIAdapter),
			((void**)&dxgiAdapter))) &&

		!__MYE_DX11_HR_TEST_FAILED(dxgiAdapter->GetParent(
			__uuidof(IDXGIFactory),
			((void**)&dxgiFactory))) &&

		!__MYE_DX11_HR_TEST_FAILED(dxgiFactory->CreateSwapChain(
			m_swapChainConfiguration.device->GetDevice(),
			&swapDesc,
			&m_swapChain))
		);

	__MYE_DX11_RELEASE_COM(dxgiDevice);
	__MYE_DX11_RELEASE_COM(dxgiAdapter);
	__MYE_DX11_RELEASE_COM(dxgiFactory);

	return success;

}

bool DX11SwapChain::CreateBackBufferView(void)
{

	ID3D11Texture2D * backBuffer;

	bool success = (
		!__MYE_DX11_HR_TEST_FAILED(m_swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer))) &&

		!__MYE_DX11_HR_TEST_FAILED(m_swapChainConfiguration.device->GetDevice()->CreateRenderTargetView(
			backBuffer,
			nullptr,
			&m_backBufferTargetView)) &&


		!__MYE_DX11_HR_TEST_FAILED(m_swapChainConfiguration.device->GetDevice()->CreateShaderResourceView(
			backBuffer,
			nullptr,
			&m_backBufferResourceView))
		);

	__MYE_DX11_RELEASE_COM(backBuffer);

	return success;

}

void DX11SwapChain::Resize(int width, int height)
{

	__MYE_DX11_RELEASE_COM(m_backBufferResourceView);
	__MYE_DX11_RELEASE_COM(m_backBufferTargetView);

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

void DX11SwapChain::ClearBackBuffer(const mye::math::Vector4f & color)
{

	m_swapChainConfiguration.device->GetImmediateContext()->ClearRenderTargetView(
		m_backBufferTargetView,
		color.Data());

}