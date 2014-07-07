#include "DX11Module.h"
#include "DX11VertexBuffer.h"
#include "DX11Font.h"
#include "DX11RasterizerState.h"

#include <mye/core/Game.h>
#include <mye/core/ResourceTypeManager.h>
#include <mye/core/Utils.h>

#include "./detail/ShadersBuffers.h"

using namespace mye::dx11;
using namespace mye::core;
using namespace mye::math;
using namespace mye::win;

DX11Module::DX11Module(void) :

	m_window(nullptr),

	m_basicRenderer             (m_device, nullptr),
	m_deferredLighthingRenderer (m_device, nullptr),
	m_text2dRenderer            (m_device)

{

	SetWindow(nullptr);
}

DX11Module::DX11Module(Window *window) :

	m_window(nullptr),

	m_basicRenderer             (m_device, nullptr),
	m_deferredLighthingRenderer (m_device, nullptr),
	m_text2dRenderer            (m_device)

{

	SetWindow(window);

}

DX11Module::~DX11Module(void)
{
	FreeWindow();
}

bool DX11Module::Init(void)
{

	bool initialized = false;

	m_window->Show();

	if (m_device.Create() &&
		m_swapChain.Create())
	{

		OnResize(m_window, m_window->GetSize());

		RasterizerInfo rasterizeInfo = { false, CullMode::BACK };
		DX11RasterizerState rasterizeState(m_device, rasterizeInfo);

		rasterizeState.Use();

		initialized = m_basicRenderer.Init() &&
			m_deferredLighthingRenderer.Init() &&
			m_text2dRenderer.Init();

	}

	return initialized;

}

void DX11Module::ShutDown(void)
{

	m_basicRenderer.Shutdown();
	m_deferredLighthingRenderer.Shutdown();
	m_text2dRenderer.Shutdown();
	
	FreeWindow();

	m_swapChain.Destroy();
	m_device.Destroy();

}

void DX11Module::Render(void)
{

	auto backBuffer = m_swapChain.GetBackBufferRenderTargetView();

	m_swapChain.ClearBackBuffer(m_clearColor);

	//m_basicRenderer.Render(backBuffer);
	m_deferredLighthingRenderer.Render(backBuffer);

	m_text2dRenderer.Render(backBuffer);

	m_swapChain->Present(1, 0);

}

void DX11Module::SetWindow(Window *window)
{

	FreeWindow();

	if (window)
	{

		m_window = window;
		m_mainWindowPointer = static_cast<IWindow*>(m_window);

		m_window->AddListener(static_cast<IWindow::Listener*>(this));

		auto clientSize = window->GetSize();

		DX11SwapChainConfiguration swapChainConf;

		swapChainConf.device     = &m_device;
		swapChainConf.window     = m_window;
		swapChainConf.format     = DataFormat::sRGBA32;
		swapChainConf.fullscreen = window->IsFullScreen();
		swapChainConf.height     = clientSize.y();
		swapChainConf.width      = clientSize.x();
		swapChainConf.msaa       = MSAA::MSAA_OFF;
		swapChainConf.refresh    = mye::math::Rational<unsigned int>(1, 60);

		m_swapChain = DX11SwapChain(swapChainConf);

		m_basicRenderer.SetWindow(window);
		m_deferredLighthingRenderer.SetWindow(window);

		if (m_device.Exists())
		{

			m_swapChain.Create();
			OnResize(window, clientSize);

		}

	}

	

}

void DX11Module::FreeWindow(void)
{

	if (m_window)
	{

		m_basicRenderer.SetWindow(nullptr);
		m_deferredLighthingRenderer.SetWindow(nullptr);

		m_window->RemoveListener(this);

		m_window = nullptr;
		m_mainWindowPointer = nullptr;

		m_swapChain.Destroy();

	}

}

void DX11Module::OnResize(IWindow *window, const mye::math::Vector2i &size)
{


	if (m_swapChain.Exists())
	{
		m_swapChain.Resize(size.x(), size.y());
	}
	
	if (m_device.Exists())
	{

		D3D11_VIEWPORT viewPort;

		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		viewPort.Width = (float)size.x();
		viewPort.Height = (float)size.y();

		m_device.GetImmediateContext()->RSSetViewports(1, &viewPort);

	}

}