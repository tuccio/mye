#include "DX11Module.h"

using namespace mye::dx11;
using namespace mye::core;

DX11Module::DX11Module(void) :
	m_device(),
	m_window(m_device)
{
	m_window.Create();
}

DX11Module::~DX11Module(void)
{
}

bool DX11Module::Init(void)
{
	m_mainWindowPointer = &m_window;
	m_window.Show();
	return m_window.Init();
}

void DX11Module::ShutDown(void)
{
	m_window.Hide();
	m_mainWindowPointer = NULL;
}

void DX11Module::Render(void)
{

	m_device.GetImmediateContext()->ClearRenderTargetView(
		m_window.GetRenderTargetView(),
		m_clearColor.Data()
		);

	m_device.GetImmediateContext()->ClearDepthStencilView(
		m_window.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	m_window.GetSwapChain()->Present(1, 0);

}