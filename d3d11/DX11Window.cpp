#include "DX11Window.h"
#include "DX11Utils.h"

#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Window::DX11Window(DX11Device &device) :
	m_device(device)
{

	m_refresh.Numerator   = 1;
	m_refresh.Denominator = 60;
	m_msaa                = MSAA_OFF;

	AddListener(&m_resizeListener);

}


DX11Window::~DX11Window(void)
{
	ShutDown();
}

bool DX11Window::Init(void)
{

	if (!CreateSwapChain())
	{
		Logger::LogErrorOptional("Cannot create swap chain");
		return false;
	}

	if (!CreateDepthStencilBuffers())
	{
		Logger::LogErrorOptional("Cannot create depth and stencil buffers");
		return false;
	}

	if (!CreateRenderTargetView())
	{
		Logger::LogErrorOptional("Cannot create render target view");
		return false;
	}

	Eigen::Vector2i clientSize = GetSize();
	SetViewport(0, 0, clientSize.x(), clientSize.y());

	return true;

}

void DX11Window::ShutDown(void)
{

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = NULL;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = NULL;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = NULL;
	}

}

bool DX11Window::CreateSwapChain()
{

	UINT msaa4xQuality, msaa8xQuality;

	DXGI_SWAP_CHAIN_DESC swapDesc;
	Eigen::Vector2i clientSize = GetSize();

	swapDesc.BufferDesc.RefreshRate      = m_refresh;
	swapDesc.BufferDesc.Width            = clientSize.x();
	swapDesc.BufferDesc.Height           = clientSize.y();
	swapDesc.BufferDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	swapDesc.BufferCount                 = 1;
	swapDesc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow                = GetHandle();
/*	swapDesc.Windowed                    = !IsFullScreen();*/
	swapDesc.Windowed                    = true;
	swapDesc.SwapEffect                  = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Flags                       = 0;

	switch (m_msaa)
	{

	case MSAA_4x:

		m_device.GetDevice()->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			4,
			&msaa4xQuality);

		assert(msaa4xQuality > 0);

		swapDesc.SampleDesc.Count   = 4;
		swapDesc.SampleDesc.Quality = msaa4xQuality - 1;

		break;

	case MSAA_8x:

		m_device.GetDevice()->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			8,
			&msaa8xQuality);

		swapDesc.SampleDesc.Count   = 8;
		swapDesc.SampleDesc.Quality = msaa8xQuality - 1;

		assert(msaa8xQuality > 0);

		break;

	default:
		swapDesc.SampleDesc.Count   = 1;
		swapDesc.SampleDesc.Quality = 0;
		break;

	}

	IDXGIDevice* dxgiDevice = 0;

	HRESULT hr1 = m_device.GetDevice()->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;

	HRESULT hr2 = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), ((void**) &dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;

	HRESULT hr3 = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), ((void**) &dxgiFactory));

	HRESULT hr4 = dxgiFactory->CreateSwapChain(m_device.GetDevice(),
		&swapDesc,
		&m_swapChain);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	HRDEBUG(hr1);
	HRDEBUG(hr2);
	HRDEBUG(hr3);
	HRDEBUG(hr4);

	return !(FAILED(hr1) || FAILED(hr2) || FAILED(hr3) || FAILED(hr4));

}

void DX11Window::SetRefreshRate(DXGI_RATIONAL refresh)
{
	m_refresh = refresh;
}

void DX11Window::SetMSAA(MSAA msaa)
{
	m_msaa = msaa;
}

void DX11Window::ResizeBuffers(int width, int height)
{

	m_renderTargetView->Release();

	m_swapChain->ResizeBuffers(1,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0);

	ReleaseCOM(m_depthStencilView);
	ReleaseCOM(m_depthStencilBuffer);

	CreateDepthStencilBuffers();
	CreateRenderTargetView();

	m_device.GetImmediateContext()->OMSetRenderTargets(1,
		&m_renderTargetView,
		m_depthStencilView);

}

void DX11Window::SetViewport(int x, int y, int width, int height)
{

	D3D11_VIEWPORT viewPort;

	viewPort.TopLeftX = x;
	viewPort.TopLeftY = y;
	viewPort.Width    = width;
	viewPort.Height   = height;
	
	m_device.GetImmediateContext()->RSSetViewports(1, &viewPort);

}

bool DX11Window::CreateRenderTargetView(void)
{

	ID3D11Texture2D *backBuffer;

	HRESULT hr1 = m_swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer));

	HRESULT hr2 = m_device.GetDevice()->CreateRenderTargetView(
		backBuffer,
		0,
		&m_renderTargetView);

	ReleaseCOM(backBuffer);

	HRDEBUG(hr1);
	HRDEBUG(hr2);

	return !(FAILED(hr1) || FAILED(hr2));

}

bool DX11Window::CreateDepthStencilBuffers(void)
{

	UINT msaa4xQuality, msaa8xQuality;

	Eigen::Vector2i clientSize = GetSize();
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width          = (clientSize.x() ? clientSize.x() : 1);
	depthStencilDesc.Height         = (clientSize.y() ? clientSize.y() : 1);
	depthStencilDesc.MipLevels      = 1;
	depthStencilDesc.ArraySize      = 1;
	depthStencilDesc.Format         = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags      = 0;

	switch (m_msaa)
	{

	case MSAA_4x:

		m_device.GetDevice()->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			8,
			&msaa4xQuality);

		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = msaa4xQuality - 1;

		break;

	case MSAA_8x:

		m_device.GetDevice()->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			8,
			&msaa8xQuality);

		depthStencilDesc.SampleDesc.Count   = 8;
		depthStencilDesc.SampleDesc.Quality = msaa8xQuality - 1;

		assert(msaa8xQuality > 0);

		break;

	default:

		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		break;

	}

	HRESULT hr1 = m_device.GetDevice()->CreateTexture2D(&depthStencilDesc,
		0,
		&m_depthStencilBuffer);

	HRESULT hr2 = m_device.GetDevice()->CreateDepthStencilView(m_depthStencilBuffer,
		0,
		&m_depthStencilView);

	HRDEBUG(hr1);
	HRDEBUG(hr2);

	return !(FAILED(hr1) || FAILED(hr2));

}

void DX11Window::ResizeListener::OnResize(IWindow *window,
										  const Eigen::Vector2i &size)
{
	DX11Window *dx11window = ((DX11Window*) window);
	dx11window->ResizeBuffers(size.x(), size.y());
	dx11window->SetViewport(0, 0, size.x(), size.y());
}