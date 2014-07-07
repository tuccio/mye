#include "DX11Device.h"
#include "DX11Utils.h"
#include "DX11VertexBuffer.h"

#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

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

	ReleaseCOMOptional(m_blendOn);
	ReleaseCOMOptional(m_blendOff);
	ReleaseCOMOptional(m_dImmediateContext);
	ReleaseCOMOptional(m_device);

}

bool DX11Device::Create(void)
{

	UINT createDeviceFlags = 0x0;
	D3D_FEATURE_LEVEL featureLevel;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	return !HRTESTFAILED(D3D11CreateDevice(
		0x0,
		D3D_DRIVER_TYPE_HARDWARE,
		0x0,
		createDeviceFlags,
		0x0,
		0x0,
		D3D11_SDK_VERSION,
		&m_device,
		&featureLevel,
		&m_dImmediateContext));

}

void DX11Device::Destroy(void)
{

	ReleaseCOMOptional(m_blendOn);
	ReleaseCOMOptional(m_blendOff);
	ReleaseCOMOptional(m_dImmediateContext);
	ReleaseCOMOptional(m_device);

}

bool DX11Device::Exists(void) const
{
	return m_device != nullptr;
}

/*
void DX11Device::Render(DX11VertexBuffer &vb)
{

	vb.Bind();

	switch (vb.GetRasterizingPrimitive())
	{
	case:
	}

	m_dImmediateContext->Draw(vb.GetVerticesCount(), 0);
}*/

void DX11Device::SetBlending(bool enable)
{

	if (!m_blendOn)
	{

		D3D11_BLEND_DESC blendStateDescription;
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		blendStateDescription.RenderTarget[0].BlendEnable           = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		HRDEBUG(m_device->CreateBlendState(&blendStateDescription, &m_blendOn));

	}

	if (!m_blendOff)
	{

		D3D11_BLEND_DESC blendStateDescription;
		ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

		blendStateDescription.RenderTarget[0].BlendEnable           = FALSE;
		blendStateDescription.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		HRDEBUG(m_device->CreateBlendState(&blendStateDescription, &m_blendOff));

	}

	m_dImmediateContext->OMSetBlendState((enable ? m_blendOn : m_blendOff), nullptr, 0xFFFFFFFF);

}

void DX11Device::SetDepthTest(bool enable)
{

	if (!m_depthTestOn)
	{

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

		HRDEBUG(m_device->CreateDepthStencilState(&depthStencilStateDescription, &m_depthTestOn));

	}

	if (!m_depthTestOff)
	{

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDescription;

		ZeroMemory(&depthStencilStateDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDescription.DepthEnable                  = FALSE;
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

		HRDEBUG(m_device->CreateDepthStencilState(&depthStencilStateDescription, &m_depthTestOff));

	}

	m_dImmediateContext->OMSetDepthStencilState((enable ? m_depthTestOn : m_depthTestOff), 0);

}