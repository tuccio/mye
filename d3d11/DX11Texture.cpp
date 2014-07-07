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
m_renderTargetView(nullptr),
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

		bool renderTarget = m_params.Contains("renderTarget") &&
			m_params.GetBool("renderTarget");

		m_format = DataFormat::FLOAT4;

		m_width  = image->GetWidth();
		m_height = image->GetHeight();

		D3D11_TEXTURE2D_DESC tex2dDesc;

		tex2dDesc.Width              = m_width;
		tex2dDesc.Height             = m_height;

		tex2dDesc.MipLevels          = 1;
		tex2dDesc.ArraySize          = 1;
		tex2dDesc.Format             = GetDXGIFormat(m_format);
		tex2dDesc.SampleDesc.Count   = 1;
		tex2dDesc.SampleDesc.Quality = 0;
		tex2dDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE |
			(renderTarget ? D3D11_BIND_RENDER_TARGET : 0x0);
		tex2dDesc.MiscFlags          = 0;
		tex2dDesc.Usage              = D3D11_USAGE_DEFAULT;
		tex2dDesc.CPUAccessFlags     = 0;

		D3D11_SUBRESOURCE_DATA data;

		data.pSysMem          = image->GetData();
		data.SysMemPitch      = m_width * 16;
		data.SysMemSlicePitch = 0;

		if (!HRTESTFAILED(m_device.GetDevice()->CreateTexture2D(&tex2dDesc, &data, &m_texture)) &&
			CreateViews())
		{

			loaded = true;

		}

	}

	return loaded;

}

void DX11Texture::Bind(unsigned int slot)
{
	m_device.GetImmediateContext()->PSSetShaderResources(slot, 1, &m_shaderResourceView);
	m_boundSlot = slot;
}

void DX11Texture::Unbind(void)
{
	ID3D11ShaderResourceView *unbound = nullptr;
	m_device.GetImmediateContext()->PSSetShaderResources(m_boundSlot, 1, &unbound);
}

void DX11Texture::UnloadImpl(void)
{

	ReleaseCOM(m_shaderResourceView);
	ReleaseCOM(m_renderTargetView);
	ReleaseCOM(m_texture);

}

size_t DX11Texture::CalculateSizeImpl(void)
{
	return m_width * m_height * 16;
}

bool DX11Texture::Create(int width, int height, mye::core::DataFormat format)
{

	m_format = format;

	m_width  = width;
	m_height = height;

	D3D11_TEXTURE2D_DESC tex2dDesc;

	tex2dDesc.Width = m_width;
	tex2dDesc.Height = m_height;

	tex2dDesc.MipLevels = 1;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.Format = GetDXGIFormat(m_format);
	tex2dDesc.SampleDesc.Count = 1;
	tex2dDesc.SampleDesc.Quality = 0;
	tex2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tex2dDesc.MiscFlags = 0;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;
	tex2dDesc.CPUAccessFlags = 0;

	if (!HRTESTFAILED(m_device.GetDevice()->CreateTexture2D(&tex2dDesc, nullptr, &m_texture)) &&
		CreateViews())
	{
		return true;
	}

	return false;

}

bool DX11Texture::CreateViews(void)
{

	bool renderTarget = m_params.Contains("renderTarget") &&
		m_params.GetBool("renderTarget");

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format                    = GetDXGIFormat(m_format);
	shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels       = 1;
	shaderResourceViewDesc.Buffer.FirstElement       = 0;
	shaderResourceViewDesc.Buffer.NumElements        = 1;

	return
		!HRTESTFAILED(
			m_device.GetDevice()->CreateShaderResourceView(
			m_texture,
			&shaderResourceViewDesc,
			&m_shaderResourceView)) &&
		(!renderTarget ||
		!HRTESTFAILED(
			m_device.GetDevice()->CreateRenderTargetView(
			m_texture,
			nullptr,
			&m_renderTargetView)));

}

void DX11Texture::Destroy(void)
{

	ReleaseCOMOptional(m_shaderResourceView);
	ReleaseCOMOptional(m_renderTargetView);
	ReleaseCOMOptional(m_texture);

}

void DX11Texture::ClearRenderTarget(const mye::math::Vector4f &color)
{
	m_device.GetImmediateContext()->ClearRenderTargetView(m_renderTargetView, color.Data());
}