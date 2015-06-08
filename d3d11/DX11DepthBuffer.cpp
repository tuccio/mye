#include "DX11DepthBuffer.h"
#include "DX11Utils.h"

using namespace mye::dx11;

DX11DepthBuffer::DX11DepthBuffer(void) :
	m_depthStencilView(nullptr),
	m_depthStencilBuffer(nullptr)
{

}

DX11DepthBuffer::DX11DepthBuffer(DX11DepthBufferConfiguration depthBufferConfiguration) :
	m_depthBufferConfiguration(depthBufferConfiguration),
	m_depthStencilView(nullptr),
	m_depthStencilBuffer(nullptr)
{
}


DX11DepthBuffer::~DX11DepthBuffer(void)
{
}


bool DX11DepthBuffer::Create(void)
{

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width              = (m_depthBufferConfiguration.width ? m_depthBufferConfiguration.width : 1);
	depthStencilDesc.Height             = (m_depthBufferConfiguration.height ? m_depthBufferConfiguration.height : 1);
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0;
	depthStencilDesc.MiscFlags          = 0;

	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	if (m_depthBufferConfiguration.shaderResource)
	{
		depthStencilDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	depthStencilViewDesc.Flags              = 0;
	depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels       = 1;

	bool success = (

		!__MYE_DX11_HR_TEST_FAILED(m_depthBufferConfiguration.device->GetDevice()->
		CreateTexture2D(
			&depthStencilDesc,
			0,
			&m_depthStencilBuffer)) &&

		!__MYE_DX11_HR_TEST_FAILED(m_depthBufferConfiguration.device->GetDevice()->
		CreateDepthStencilView(
			m_depthStencilBuffer,
			&depthStencilViewDesc,
			&m_depthStencilView)) &&

		(!m_depthBufferConfiguration.shaderResource ||
		!__MYE_DX11_HR_TEST_FAILED(m_depthBufferConfiguration.device->GetDevice()->
		CreateShaderResourceView(
			m_depthStencilBuffer,
			&shaderResourceViewDesc,
			&m_shaderResourceView)))

		);

	return success;

}

void DX11DepthBuffer::Destroy(void)
{

	__MYE_DX11_RELEASE_COM_OPTIONAL(m_depthStencilView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_depthStencilBuffer);

}

void DX11DepthBuffer::Resize(int width, int height)
{

	m_depthBufferConfiguration.width  = width;
	m_depthBufferConfiguration.height = height;

	if (m_depthStencilBuffer)
	{
		Destroy();
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