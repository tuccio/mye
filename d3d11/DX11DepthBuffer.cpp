#include "DX11DepthBuffer.h"
#include "DX11Utils.h"

using namespace mye::dx11;

DX11DepthBuffer::DX11DepthBuffer(void)
{

}

DX11DepthBuffer::DX11DepthBuffer(DX11DepthBufferConfiguration depthBufferConfiguration) :
	m_depthBufferConfiguration(depthBufferConfiguration)
{
}


DX11DepthBuffer::~DX11DepthBuffer(void)
{
}


bool DX11DepthBuffer::Create(void)
{

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = (m_depthBufferConfiguration.width ? m_depthBufferConfiguration.width : 1);
	depthStencilDesc.Height = (m_depthBufferConfiguration.height ? m_depthBufferConfiguration.height : 1);
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	bool success = (
		!HRTESTFAILED(m_depthBufferConfiguration.device->GetDevice()->
		CreateTexture2D(
		&depthStencilDesc,
		0,
		&m_depthStencilBuffer)) &&

		!HRTESTFAILED(m_depthBufferConfiguration.device->GetDevice()->
		CreateDepthStencilView(
		m_depthStencilBuffer,
		0,
		&m_depthStencilView))
		);

	return success;

}

void DX11DepthBuffer::Destroy(void)
{

	ReleaseCOMOptional(m_depthStencilView);
	ReleaseCOMOptional(m_depthStencilBuffer);

}

void DX11DepthBuffer::Resize(int width, int height)
{

	m_depthBufferConfiguration.width = width;
	m_depthBufferConfiguration.height = height;

	if (m_depthStencilBuffer)
	{

		ReleaseCOM(m_depthStencilView);
		ReleaseCOM(m_depthStencilBuffer);

		Create();

	}

}

void DX11DepthBuffer::Clear(float depth)
{

	m_depthBufferConfiguration.device->GetImmediateContext()->ClearDepthStencilView(
		m_depthStencilView,
		D3D11_CLEAR_DEPTH,
		depth,
		0);

}