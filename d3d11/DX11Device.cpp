#include "DX11Device.h"
#include "DX11Utils.h"
#include "DX11VertexBuffer.h"

#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

#if _DEBUG
#define MYE_DX11_DEVICE_DEBUG
#endif

DX11Device::DX11Device(void) :
	m_device(nullptr),
	m_blendOn(nullptr),
	m_blendOff(nullptr),
	m_depthTestOff(nullptr),
	m_depthTestOn(nullptr)
{

}


DX11Device::~DX11Device(void)
{
}

bool DX11Device::Create(void)
{

	UINT createDeviceFlags = 0x0;
	D3D_FEATURE_LEVEL featureLevel;

#ifdef MYE_DX11_DEVICE_DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (!__MYE_DX11_HR_TEST_FAILED(D3D11CreateDevice(
		0x0,
		D3D_DRIVER_TYPE_HARDWARE,
		0x0,
		createDeviceFlags,
		0x0,
		0x0,
		D3D11_SDK_VERSION,
		&m_device,
		&featureLevel,
		&m_dImmediateContext)))
	{

		/* Create blend states */

		D3D11_BLEND_DESC blendStateDescription;
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		blendStateDescription.RenderTarget[0].BlendEnable           = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		__MYE_DX11_HR_DEBUG(m_device->CreateBlendState(&blendStateDescription, &m_blendOn));

		blendStateDescription.RenderTarget[0].BlendEnable           = FALSE;

		__MYE_DX11_HR_DEBUG(m_device->CreateBlendState(&blendStateDescription, &m_blendOff));

		/* Create depth states */

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDescription;

		ZeroMemory(&depthStencilStateDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDescription.DepthEnable                  = TRUE;
		depthStencilStateDescription.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDescription.DepthFunc                    = D3D11_COMPARISON_LESS;

		depthStencilStateDescription.StencilEnable                = FALSE;
		depthStencilStateDescription.StencilReadMask              = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilStateDescription.StencilWriteMask             = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		depthStencilStateDescription.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		depthStencilStateDescription.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

		depthStencilStateDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescription.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescription.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescription.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescription.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		depthStencilStateDescription.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;

		__MYE_DX11_HR_DEBUG(m_device->CreateDepthStencilState(&depthStencilStateDescription, &m_depthTestOn));

		depthStencilStateDescription.DepthEnable = FALSE;

		__MYE_DX11_HR_DEBUG(m_device->CreateDepthStencilState(&depthStencilStateDescription, &m_depthTestOff));

		depthStencilStateDescription.DepthEnable    = TRUE;
		depthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthStencilStateDescription.DepthFunc      = D3D11_COMPARISON_EQUAL;

		__MYE_DX11_HR_DEBUG(m_device->CreateDepthStencilState(&depthStencilStateDescription, &m_depthTestLookup));

		return true;

	}
	else
	{
		return false;
	}

}

void DX11Device::Destroy(void)
{

	__MYE_DX11_RELEASE_COM_OPTIONAL(m_blendOn);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_blendOff);

	__MYE_DX11_RELEASE_COM_OPTIONAL(m_depthTestOn);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_depthTestOff);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_depthTestLookup);

	__MYE_DX11_RELEASE_COM_OPTIONAL(m_dImmediateContext);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_device);

}

bool DX11Device::Exists(void) const
{
	return m_device != nullptr;
}

void DX11Device::SetBlending(bool enable)
{
	m_dImmediateContext->OMSetBlendState((enable ? m_blendOn : m_blendOff), nullptr, 0xFFFFFFFF);
}

void DX11Device::SetDepthTest(DX11DepthTest test)
{

	switch (test)
	{

	case DX11DepthTest::ON:
		m_dImmediateContext->OMSetDepthStencilState(m_depthTestOn, 0);
		break;

	case DX11DepthTest::OFF:
		m_dImmediateContext->OMSetDepthStencilState(m_depthTestOff, 0);
		break;

	case DX11DepthTest::LOOKUP:
		m_dImmediateContext->OMSetDepthStencilState(m_depthTestLookup, 0);
		break;

	}

}

DXGI_SAMPLE_DESC DX11Device::GetMSAASampleDesc(MSAA msaa, DXGI_FORMAT format)
{

	DXGI_SAMPLE_DESC sampleDesc;

	switch (msaa)
	{

	case MSAA::MSAA_2X:

	{

		UINT msaa2xQuality;

		m_device->CheckMultisampleQualityLevels(format, 2, &msaa2xQuality);

		sampleDesc.Count   = 2;
		sampleDesc.Quality = msaa2xQuality - 1;

	}

		break;

	case MSAA::MSAA_4X:

	{

		UINT msaa4xQuality;

		m_device->CheckMultisampleQualityLevels(format, 4, &msaa4xQuality);

		sampleDesc.Count   = 4;
		sampleDesc.Quality = msaa4xQuality - 1;

	}

		break;

	case MSAA::MSAA_8X:

	{

		UINT msaa8xQuality;

		m_device->CheckMultisampleQualityLevels(format,	8, &msaa8xQuality);

		sampleDesc.Count   = 8;
		sampleDesc.Quality = msaa8xQuality - 1;

	}

		break;

	case MSAA::MSAA_16X:

	{

		UINT msaa16xQuality;

		m_device->CheckMultisampleQualityLevels(format,	16,	&msaa16xQuality);

		sampleDesc.Count   = 16;
		sampleDesc.Quality = msaa16xQuality - 1;

	}

		break;

	default:

		sampleDesc.Count   = 1;
		sampleDesc.Quality = 0;

		break;

	}

	return sampleDesc;

}

std::vector<D3D11_VIEWPORT> DX11Device::GetViewports(void)
{

	D3D11_VIEWPORT oldViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	unsigned int numViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

	GetImmediateContext()->RSGetViewports(&numViewports, oldViewports);
	return std::vector<D3D11_VIEWPORT>(oldViewports, oldViewports + numViewports);

}

void DX11Device::SetViewports(const D3D11_VIEWPORT * viewports, unsigned int numViewports)
{
	GetImmediateContext()->RSSetViewports(numViewports, viewports);
}