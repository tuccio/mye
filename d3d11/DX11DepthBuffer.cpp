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

	depthStencilDesc.Width          = (m_depthBufferConfiguration.width ? m_depthBufferConfiguration.width : 1);
	depthStencilDesc.Height         = (m_depthBufferConfiguration.height ? m_depthBufferConfiguration.height : 1);
	depthStencilDesc.MipLevels      = 1;
	depthStencilDesc.ArraySize      = m_depthBufferConfiguration.arraySize;
	depthStencilDesc.Format         = DXGI_FORMAT_R32_TYPELESS;
								    
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags      = 0;

	depthStencilDesc.SampleDesc     = DX11Device::GetSingleton().GetMSAASampleDesc(m_depthBufferConfiguration.msaa, DXGI_FORMAT_R32_TYPELESS);

	if (m_depthBufferConfiguration.shaderResource)
	{
		depthStencilDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	depthStencilViewDesc.Flags  = 0;
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	
	
	if (m_depthBufferConfiguration.arraySize > 1)
	{

		if (m_depthBufferConfiguration.msaa == MSAA::MSAA_OFF)
		{

			depthStencilViewDesc.ViewDimension                    = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			depthStencilViewDesc.Texture2DArray.ArraySize         = m_depthBufferConfiguration.arraySize;
			depthStencilViewDesc.Texture2DArray.FirstArraySlice   = 0;
			depthStencilViewDesc.Texture2DArray.MipSlice          = 0;

			shaderResourceViewDesc.ViewDimension                  = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			shaderResourceViewDesc.Texture2DArray.ArraySize       = m_depthBufferConfiguration.arraySize;
			shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
			shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2DArray.MipLevels       = 1;

		}
		else
		{
			
			depthStencilViewDesc.ViewDimension                      = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
			depthStencilViewDesc.Texture2DMSArray.ArraySize         = m_depthBufferConfiguration.arraySize;
			depthStencilViewDesc.Texture2DMSArray.FirstArraySlice   = 0;

			shaderResourceViewDesc.ViewDimension                    = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
			shaderResourceViewDesc.Texture2DMSArray.ArraySize       = m_depthBufferConfiguration.arraySize;
			shaderResourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;

		}

	}
	else
	{

		if (m_depthBufferConfiguration.msaa == MSAA::MSAA_OFF)
		{

			depthStencilViewDesc.ViewDimension               = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice          = 0;

			shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels       = 1;

		}
		else
		{
			depthStencilViewDesc.ViewDimension               = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}

	}	

	bool success = (

		!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
		CreateTexture2D(
			&depthStencilDesc,
			0,
			&m_depthStencilBuffer)) &&

			!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
		CreateDepthStencilView(
			m_depthStencilBuffer,
			&depthStencilViewDesc,
			&m_depthStencilView)) &&

		(!m_depthBufferConfiguration.shaderResource ||
		!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->
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

bool DX11DepthBuffer::Resize(int width, int height)
{

	m_depthBufferConfiguration.width  = width;
	m_depthBufferConfiguration.height = height;

	if (m_depthStencilBuffer)
	{
		Destroy();
		return Create();
	}

	return false;

}

bool DX11DepthBuffer::ResizeArray(int size)
{
	m_depthBufferConfiguration.arraySize = size;
	Destroy();
	return Create();
}

bool DX11DepthBuffer::SetMSAA(MSAA msaa)
{
	m_depthBufferConfiguration.msaa = msaa;
	Destroy();
	return Create();
}

MSAA DX11DepthBuffer::GetMSAA(void) const
{
	return m_depthBufferConfiguration.msaa;
}

void DX11DepthBuffer::Clear(float depth)
{

	DX11Device::GetSingleton().GetImmediateContext()->ClearDepthStencilView(
		m_depthStencilView,
		D3D11_CLEAR_DEPTH,
		depth,
		0);

}