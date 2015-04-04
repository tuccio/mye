#include "DX11VolumeTexture.h"

using namespace mye::dx11;
using namespace mye::core;

DX11VolumeTexture::DX11VolumeTexture(DX11Device & device) :
	m_device(device) { }

bool DX11VolumeTexture::Create(int width, int height, int depth, DataFormat format)
{

	D3D11_TEXTURE3D_DESC t3dDesc;

	t3dDesc.Width          = width;
	t3dDesc.Height         = height;
	t3dDesc.Depth          = depth;

	t3dDesc.MipLevels      = 0;
	t3dDesc.Format         = GetDXGIFormat(format);
	t3dDesc.Usage          = D3D11_USAGE_DEFAULT;
	t3dDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	t3dDesc.CPUAccessFlags = 0x0;
	t3dDesc.MiscFlags      = 0x0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	return
		!__MYE_DX11_HR_TEST_FAILED(m_device->CreateTexture3D(&t3dDesc, nullptr, &m_texture)) &&
		!__MYE_DX11_HR_TEST_FAILED(m_device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView)) &&
		!__MYE_DX11_HR_TEST_FAILED(m_device->CreateRenderTargetView(m_texture, nullptr, &m_renderTargetView));

}

void DX11VolumeTexture::Destroy(void)
{
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_renderTargetView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shaderResourceView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_texture);
}