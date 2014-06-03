#include "DX11Device.h"
#include "DX11Utils.h"
#include "DX11VertexBuffer.h"

#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Device::DX11Device(void) :
	m_blendOn(nullptr),
	m_blendOff(nullptr)
{

	UINT createDeviceFlags = 0x0;
	D3D_FEATURE_LEVEL featureLevel;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	;

	if (HRTESTFAILED(D3D11CreateDevice(
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
		throw;
	}

}


DX11Device::~DX11Device(void)
{

	ReleaseCOMIf(m_blendOn);
	ReleaseCOMIf(m_blendOff);
	ReleaseCOMIf(m_dImmediateContext);
	ReleaseCOMIf(m_device);

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