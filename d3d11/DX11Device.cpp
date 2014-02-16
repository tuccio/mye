#include "DX11Device.h"
#include "DX11Utils.h"
#include "DX11VertexBuffer.h"

#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Device::DX11Device(void)
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