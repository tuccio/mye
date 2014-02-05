#include "DX11Module.h"

using namespace mye::dx11;
using namespace mye::core;

DX11Module::DX11Module(void) :
	m_ownedDevice(false)
{
	NewWindow();
}

DX11Module::DX11Module(DX11Window *window) :
	m_ownedDevice(false)
{
	SetWindow(window);
}

DX11Module::~DX11Module(void)
{
	FreeWindow();
}

bool DX11Module::Init(void)
{
	m_window->Show();
	return m_window->Init();
}

void DX11Module::ShutDown(void)
{
	m_window->Hide();
}

void DX11Module::Render(void)
{

	m_device->GetImmediateContext()->ClearRenderTargetView(
		m_window->GetRenderTargetView(),
		m_clearColor.Data()
		);

	m_device->GetImmediateContext()->ClearDepthStencilView(
		m_window->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	m_window->GetSwapChain()->Present(1, 0);

}

void DX11Module::SetWindow(DX11Window *window)
{

	FreeWindow();

	m_device = &(window->GetDevice());
	m_window = window;
	m_ownedDevice = false;

	m_mainWindowPointer = m_window;

}

void DX11Module::NewWindow(void)
{

	FreeWindow();

	m_device = new DX11Device;
	m_window = new DX11Window(*m_device);
	m_ownedDevice = true;
	m_window->Create();

	m_mainWindowPointer = m_window;

}

void DX11Module::FreeWindow(void)
{

	if (m_ownedDevice)
	{
		delete m_window;
		delete m_device;
	}

	m_window = NULL;
	m_device = NULL;
	m_ownedDevice = false;

}