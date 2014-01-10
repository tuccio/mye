#include "DX11Device.h"

#include <mye/core/Logger.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Device::DX11Device(void)
{

	UINT createDeviceFlags = 0x0;
	D3D_FEATURE_LEVEL featureLevel;

#ifdef DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hResult = D3D11CreateDevice(
		0x0,
		D3D_DRIVER_TYPE_HARDWARE,
		0x0,
		createDeviceFlags,
		0x0,
		0x0,
		D3D11_SDK_VERSION,
		&m_device,
		&featureLevel,
		&m_dImmediateContext);

	if (FAILED(hResult))
	{

		Logger::LogErrorOptional("D3D11CreateDevice failed");

		if (featureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			Logger::LogErrorOptional("DirectX 11 unsupported");
		}

		throw;
	}

}


DX11Device::~DX11Device(void)
{
}
