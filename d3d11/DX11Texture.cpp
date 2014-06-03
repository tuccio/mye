#include "DX11Texture.h"

#include "DX11Utils.h"

#include <mye/core/Image.h>
#include <mye/core/ResourceTypeManager.h>

#include <D3D11.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Texture::DX11Texture(
	mye::core::ResourceManager *owner,
	const mye::core::String &name,
	mye::core::ManualResourceLoader *manual,
	DX11Device &device) :
m_device(device),
m_texture(nullptr),
m_shaderResourceView(nullptr),
//m_samplerState(nullptr),
Texture(owner, name, manual)
{

}


DX11Texture::~DX11Texture(void)
{
}

bool DX11Texture::LoadImpl(void)
{

	bool loaded = false;

	ImagePointer image = ResourceTypeManager::GetSingleton().CreateResource<Image>("Image", m_name);

	if (image && image->Load())
	{

		m_imageWidth  = image->GetWidth();
		m_imageHeight = image->GetHeight();

		D3D11_TEXTURE2D_DESC tex2dDesc;

		tex2dDesc.Width              = m_imageWidth;
		tex2dDesc.Height             = m_imageHeight;

		tex2dDesc.MipLevels          = 1;
		tex2dDesc.ArraySize          = 1;
		tex2dDesc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tex2dDesc.SampleDesc.Count   = 1;
		tex2dDesc.SampleDesc.Quality = 0;
		tex2dDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		tex2dDesc.MiscFlags          = 0;
		tex2dDesc.Usage              = D3D11_USAGE_DEFAULT;
		tex2dDesc.CPUAccessFlags     = 0;

		D3D11_SUBRESOURCE_DATA data;

		data.pSysMem          = image->GetData();
		data.SysMemPitch      = m_imageWidth * 16;
		data.SysMemSlicePitch = 0;

		if (!HRTESTFAILED(m_device.GetDevice()->CreateTexture2D(&tex2dDesc, &data, &m_texture)))
		{

			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

			shaderResourceViewDesc.Format                    = DXGI_FORMAT_R32G32B32A32_FLOAT;
			shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels       = 1;
			shaderResourceViewDesc.Buffer.FirstElement       = 0;
			shaderResourceViewDesc.Buffer.NumElements        = 1;

			if (!HRTESTFAILED(m_device.GetDevice()->CreateShaderResourceView(m_texture, &shaderResourceViewDesc, &m_shaderResourceView)))
			{
				loaded = true;
			}

		}

	}

	

	return loaded;

}

void DX11Texture::Bind(unsigned int slot)
{

	//m_device.GetImmediateContext()->PSSetSamplers(slot, 1, &m_samplerState);
	m_device.GetImmediateContext()->PSSetShaderResources(0, 1, &m_shaderResourceView);

}

void DX11Texture::UnloadImpl(void)
{

	ReleaseCOM(m_shaderResourceView);
	ReleaseCOM(m_texture);

}

size_t DX11Texture::CalculateSizeImpl(void)
{
	return m_imageWidth * m_imageHeight * 16;
}