#include "DX11Texture.h"

#include "DX11Utils.h"

#include <mye/core/Image.h>
#include <mye/core/ResourceTypeManager.h>

#include <D3D11.h>

#undef max

using namespace mye::dx11;
using namespace mye::core;

static ID3D11Texture2D * __CreateMipmappedTexture(ImagePointer image);

DX11Texture::DX11Texture(ResourceManager      * owner,
                         const String         & name,
                         ManualResourceLoader * manual) :
	m_texture2d(nullptr),
	m_msaa(MSAA::MSAA_OFF),
	Texture(owner, name, manual) { }

DX11Texture::DX11Texture(void) :
	m_texture2d(nullptr),
	m_msaa(MSAA::MSAA_OFF),
	Texture(nullptr, "", nullptr) { }

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

		int mipmaps     = m_params.Contains("mipmaps") ? m_params.GetInteger("mipmaps") : 1;
		int slices      = m_params.Contains("slices") ? m_params.GetInteger("slices") : 1;
		int msaaSamples = m_params.Contains("msaa") ? m_params.GetInteger("msaa") : 0;

		m_msaa = static_cast<MSAA>(msaaSamples);

		m_width  = image->GetWidth();
		m_height = image->GetHeight();

		D3D11_TEXTURE2D_DESC tex2dDesc;

		tex2dDesc.Width              = m_width;
		tex2dDesc.Height             = m_height;

		tex2dDesc.MipLevels          = mipmaps;
		tex2dDesc.ArraySize          = slices;
		tex2dDesc.Format             = dxgiFormat;
		
		tex2dDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
		tex2dDesc.MiscFlags          = 0;
		tex2dDesc.Usage              = D3D11_USAGE_IMMUTABLE;
		tex2dDesc.CPUAccessFlags     = 0;
		tex2dDesc.SampleDesc         = DX11Device::GetSingleton().GetMSAASampleDesc(m_msaa, dxgiFormat);

		if (renderTarget)
		{

			tex2dDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			tex2dDesc.Usage      = D3D11_USAGE_DEFAULT;

			if (generateMips)
			{
				tex2dDesc.MipLevels  = 0;
				tex2dDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			}

		}
		else if (generateMips)
		{

			tex2dDesc.MipLevels  = 0;
			tex2dDesc.Usage      = D3D11_USAGE_DEFAULT;
			tex2dDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			tex2dDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

			if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetDevice()->CreateTexture2D(&tex2dDesc, nullptr, &m_texture2d)) &&
				__CreateViews())
			{

				unsigned int   rowPitch = m_width * GetDataTypeSize(m_format);
				const void   * data     = image->GetData();

				DX11Device::GetSingleton().GetImmediateContext()->UpdateSubresource(m_texture2d, 0, nullptr, data, rowPitch, 0);
				GenerateMips();
				loaded = true;
			}

		}
		else
		{

			D3D11_SUBRESOURCE_DATA dataDesc;

			dataDesc.pSysMem          = image->GetData();
			dataDesc.SysMemPitch      = m_width * GetDataTypeSize(m_format);
			dataDesc.SysMemSlicePitch = 0;

			if (!__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetDevice()->CreateTexture2D(&tex2dDesc, &dataDesc, &m_texture2d)) &&
				__CreateViews())
			{
				loaded = true;
			}

		}

	}

	return loaded;

}

void DX11Texture::UnloadImpl(void)
{

	Destroy();

}

size_t DX11Texture::CalculateSizeImpl(void)
{
	int count = m_params.Contains("slices") ? m_params.GetInteger("slices") : 1;
	return m_width * m_height * count * GetDataTypeSize(m_format);
}

bool DX11Texture::Create(int width, int height, DataFormat format, void * data)
{

	m_format = format;

	m_width  = std::max(1, width);
	m_height = std::max(1, height);

	DXGI_FORMAT dxgiFormat = GetDXGIFormat(m_format);

	bool renderTarget        = m_params.Contains("renderTarget") && m_params.GetBool("renderTarget");
	bool generateMips        = m_params.Contains("generateMips") && m_params.GetBool("generateMips");
	bool unorderedAccessView = m_params.Contains("unorderedAccessView") && m_params.GetBool("unorderedAccessView");

	int mipmaps = m_params.Contains("mipmaps") ? m_params.GetInteger("mipmaps") : 1;
	int slices  = m_params.Contains("slices")  ? m_params.GetInteger("slices")  : 1;
	int depth   = m_params.Contains("depth")   ? m_params.GetInteger("depth")   : 1;
	String type = m_params.Contains("type")    ? m_params.GetString("type")     : "2d";

	SetMSAA();

	bool textureCreated = false;

	D3D11_SUBRESOURCE_DATA dataDesc;

	dataDesc.pSysMem          = data;
	dataDesc.SysMemPitch      = m_width * GetDataTypeSize(m_format);
	dataDesc.SysMemSlicePitch = 0;

	if (type == "2d")
	{

		D3D11_TEXTURE2D_DESC tex2dDesc;

		tex2dDesc.Width          = m_width;
		tex2dDesc.Height         = m_height;

		tex2dDesc.MipLevels      = mipmaps;
		tex2dDesc.ArraySize      = slices;
		tex2dDesc.Format         = dxgiFormat;

		tex2dDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
		tex2dDesc.MiscFlags      = 0;
		tex2dDesc.Usage          = D3D11_USAGE_DEFAULT;
		tex2dDesc.CPUAccessFlags = 0;
		tex2dDesc.SampleDesc     = DX11Device::GetSingleton().GetMSAASampleDesc(m_msaa, dxgiFormat);

		if (unorderedAccessView)
		{
			tex2dDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if (renderTarget)
		{

			if (generateMips)
			{
				tex2dDesc.MipLevels  = 0;
				tex2dDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			}

			tex2dDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		}
		else if (generateMips)
		{
			tex2dDesc.MipLevels  = 0;
		}

		textureCreated = !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetDevice()->CreateTexture2D(&tex2dDesc, (data ? &dataDesc : nullptr), &m_texture2d));

	}
	else if (type == "3d")
	{

		D3D11_TEXTURE3D_DESC tex3dDesc;

		tex3dDesc.Width          = m_width;
		tex3dDesc.Height         = m_height;
		tex3dDesc.Depth          = depth;

		tex3dDesc.MipLevels      = mipmaps;
		tex3dDesc.Format         = dxgiFormat;

		tex3dDesc.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
		tex3dDesc.MiscFlags      = 0;
		tex3dDesc.Usage          = D3D11_USAGE_DEFAULT;
		tex3dDesc.CPUAccessFlags = 0;

		if (unorderedAccessView)
		{
			tex3dDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}

		if (renderTarget)
		{

			if (generateMips)
			{
				tex3dDesc.MipLevels  = 0;
				tex3dDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
			}

			tex3dDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		}

		textureCreated = !__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton().GetDevice()->CreateTexture3D(&tex3dDesc, (data ? &dataDesc : nullptr), &m_texture3d));

	}

	return textureCreated && __CreateViews();

}

bool DX11Texture::__CreateViews(void)
{

	bool renderTarget        = m_params.Contains("renderTarget") && m_params.GetBool("renderTarget");
	bool unorderedAccessView = m_params.Contains("unorderedAccessView") && m_params.GetBool("unorderedAccessView");

	int  slices       = m_params.Contains("slices") ? m_params.GetInteger("slices") : 1;
	String type       = m_params.Contains("type")   ? m_params.GetString("type")    : "2d";

	D3D11_SHADER_RESOURCE_VIEW_DESC  shaderResourceViewDesc;
	
	DXGI_FORMAT format = GetDXGIFormat(m_format);

	shaderResourceViewDesc.Format  = format;

	ID3D11Resource * resource = nullptr;

	if (type == "2d")
	{

		if (slices == 1)
		{

			m_textureType = D3D11_SRV_DIMENSION_TEXTURE2D;

			shaderResourceViewDesc.ViewDimension             = (m_msaa == MSAA::MSAA_OFF ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS);

			shaderResourceViewDesc.Texture2D.MostDetailedMip =  0;
			shaderResourceViewDesc.Texture2D.MipLevels       = -1;

		}
		else
		{

			m_textureType = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

			shaderResourceViewDesc.ViewDimension                  = (m_msaa == MSAA::MSAA_OFF ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY);

			shaderResourceViewDesc.Texture2DArray.ArraySize       = slices;
			shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
			shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2DArray.MipLevels       = -1;

		}

		resource = (ID3D11Resource *) m_texture2d;

	}
	else if (type == "3d")
	{

		m_textureType = D3D11_SRV_DIMENSION_TEXTURE3D;

		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;

		shaderResourceViewDesc.Texture3D.MipLevels       = -1;
		shaderResourceViewDesc.Texture3D.MostDetailedMip = 0;

		resource = (ID3D11Resource *) m_texture3d;

	}

	return
		!__MYE_DX11_HR_TEST_FAILED(
		DX11Device::GetSingleton()->CreateShaderResourceView(resource,
			                                                 &shaderResourceViewDesc,
			                                                 &m_shaderResourceView)) &&
		(!renderTarget ||
		!__MYE_DX11_HR_TEST_FAILED(
		DX11Device::GetSingleton()->CreateRenderTargetView(resource,
			                                               nullptr,
			                                               &m_renderTargetView))) &&

		(!unorderedAccessView ||
		!__MYE_DX11_HR_TEST_FAILED(
		DX11Device::GetSingleton()->CreateUnorderedAccessView(resource,
			                                                  nullptr,
			                                                  &m_unorderedAccessView)));

}

void DX11Texture::Destroy(void)
{

	__MYE_DX11_RELEASE_COM_OPTIONAL(m_shaderResourceView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_renderTargetView);
	__MYE_DX11_RELEASE_COM_OPTIONAL(m_unorderedAccessView);

	switch (m_textureType)
	{

	case D3D11_SRV_DIMENSION_TEXTURE2D:
	case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
		__MYE_DX11_RELEASE_COM_OPTIONAL(m_texture2d);
		break;

	case D3D11_SRV_DIMENSION_TEXTURE3D:
		__MYE_DX11_RELEASE_COM_OPTIONAL(m_texture3d);
		break;

	}

}

void DX11Texture::SetMSAA(void)
{

	int msaaSamples = m_params.Contains("msaa") ? m_params.GetInteger("msaa") : 0;

	switch (msaaSamples)
	{

	case 2:
		m_msaa = MSAA::MSAA_2X;
		break;

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

void DX11Texture::GenerateMips(void)
{
	DX11Device::GetSingleton().GetImmediateContext()->GenerateMips(m_shaderResourceView);
}

ID3D11Texture2D * DX11Texture::GetTexture2D(void) const
{
	return m_texture2d;
}

ID3D11Texture3D * DX11Texture::GetTexture3D(void) const
{
	return m_texture3d;
}

bool DX11Texture::Resolve(DX11Texture & destination)
{

	if (destination.m_width != m_width ||
		destination.m_height != m_height ||
		destination.m_format != m_format)
	{

		Parameters params = GetParametersList();
		params.Remove("msaa");

		destination.SetParametersList(params);

		if (!destination.Create(m_width, m_height, m_format))
		{
			return false;
		}

	}

	DX11Device::GetSingleton().GetImmediateContext()->ResolveSubresource(destination.m_texture2d, 0, m_texture2d, 0, GetDXGIFormat(m_format));

	return true;

}