#include "DX11Texture.h"

#include "DX11Utils.h"

#include <mye/core/Image.h>
#include <mye/core/ResourceTypeManager.h>

#include <D3D11.h>

using namespace mye::dx11;
using namespace mye::core;

DX11Texture::DX11Texture(mye::core::ResourceManager *owner,
						 const mye::core::String &name,
						 mye::core::ManualResourceLoader *manual,
						 DX11Device &device) :
m_device(device),
m_texture(nullptr),
m_msaa(MSAA::MSAA_OFF),
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

		m_format = DataFormat::FLOAT4;

		DXGI_FORMAT dxgiFormat = GetDXGIFormat(m_format);

		bool renderTarget = m_params.Contains("renderTarget") && m_params.GetBool("renderTarget");
		bool generateMips = m_params.Contains("generateMips") && m_params.GetBool("generateMips");

		int mipmaps = m_params.Contains("mipmaps") ? m_params.GetInteger("mipmaps") : 1;
		int msaaSamples = m_params.Contains("msaa") ? m_params.GetInteger("msaa") : 0;

		m_msaa = static_cast<MSAA>(msaaSamples);

		m_width  = image->GetWidth();
		m_height = image->GetHeight();

		D3D11_TEXTURE2D_DESC tex2dDesc;

		tex2dDesc.Width              = m_width;
		tex2dDesc.Height             = m_height;

		tex2dDesc.MipLevels          = mipmaps;
		tex2dDesc.ArraySize          = 1;
		tex2dDesc.Format             = dxgiFormat;
		
		tex2dDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		tex2dDesc.MiscFlags          = 0;
		tex2dDesc.Usage              = D3D11_USAGE_IMMUTABLE;
		tex2dDesc.CPUAccessFlags     = 0;
		tex2dDesc.SampleDesc         = m_device.GetMSAASampleDesc(m_msaa, dxgiFormat);

		if (renderTarget)
		{

			if (generateMips)
			{
				tex2dDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			}

			tex2dDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			tex2dDesc.Usage      = D3D11_USAGE_DEFAULT;

		}

		D3D11_SUBRESOURCE_DATA data;

		data.pSysMem          = image->GetData();
		data.SysMemPitch      = m_width * 16;
		data.SysMemSlicePitch = 0;

		if (!__MYE_DX11_HR_TEST_FAILED(m_device.GetDevice()->CreateTexture2D(&tex2dDesc, &data, &m_texture)) &&
			CreateViews())
		{
			loaded = true;
		}

	}

	return loaded;

}

void DX11Texture::UnloadImpl(void)
{

	__MYE_DX11_RELEASE_COM(m_shaderResourceView);
	__MYE_DX11_RELEASE_COM(m_renderTargetView);
	__MYE_DX11_RELEASE_COM(m_texture);

}

size_t DX11Texture::CalculateSizeImpl(void)
{
	return m_width * m_height * 16;
}

bool DX11Texture::Create(int width, int height, mye::core::DataFormat format)
{

	m_format = format;

	m_width  = max(1, width);
	m_height = max(1, height);

	DXGI_FORMAT dxgiFormat = GetDXGIFormat(m_format);

	bool renderTarget = m_params.Contains("renderTarget") && m_params.GetBool("renderTarget");
	bool generateMips = m_params.Contains("generateMips") && m_params.GetBool("generateMips");

	int mipmaps = m_params.Contains("mipmaps") ? m_params.GetInteger("mipmaps") : 1;
	
	SetMSAA();

	D3D11_TEXTURE2D_DESC tex2dDesc;

	tex2dDesc.Width              = m_width;
	tex2dDesc.Height             = m_height;

	tex2dDesc.MipLevels          = mipmaps;
	tex2dDesc.ArraySize          = 1;
	tex2dDesc.Format             = dxgiFormat;

	tex2dDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
	tex2dDesc.MiscFlags          = 0;
	tex2dDesc.Usage              = D3D11_USAGE_DEFAULT;
	tex2dDesc.CPUAccessFlags     = 0;
	tex2dDesc.SampleDesc         = m_device.GetMSAASampleDesc(m_msaa, dxgiFormat);
	

	if (renderTarget)
	{
		
		if (generateMips)
		{
			tex2dDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		tex2dDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

	}

	return
		!__MYE_DX11_HR_TEST_FAILED(m_device.GetDevice()->CreateTexture2D(&tex2dDesc, nullptr, &m_texture)) &&
		CreateViews();

}

bool DX11Texture::CreateViews(void)
{

	bool renderTarget = m_params.Contains("renderTarget") && m_params.GetBool("renderTarget");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format                    = GetDXGIFormat(m_format);
	shaderResourceViewDesc.ViewDimension             = (m_msaa == MSAA::MSAA_OFF ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS);

	shaderResourceViewDesc.Texture2D.MostDetailedMip =  0;
	shaderResourceViewDesc.Texture2D.MipLevels       = -1;

	return
		!__MYE_DX11_HR_TEST_FAILED(
			m_device.GetDevice()->CreateShaderResourceView(
			m_texture,
			&shaderResourceViewDesc,
			&m_shaderResourceView)) &&
		(!renderTarget ||
		!__MYE_DX11_HR_TEST_FAILED(
			m_device.GetDevice()->CreateRenderTargetView(
			m_texture,
			nullptr,
			&m_renderTargetView)));

}

void DX11Texture::Destroy(void)
{

	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shaderResourceView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_renderTargetView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_texture);

}

void DX11Texture::SetMSAA(void)
{

	int msaaSamples = m_params.Contains("msaa") ? m_params.GetInteger("msaa") : 0;

	switch (msaaSamples)
	{

	case 4:
		m_msaa = MSAA::MSAA_4X;
		break;

	case 8:
		m_msaa = MSAA::MSAA_8X;
		break;

	case 16:
		m_msaa = MSAA::MSAA_16X;
		break;

	default:
		m_msaa = MSAA::MSAA_OFF;
		break;

	}

	return;

}