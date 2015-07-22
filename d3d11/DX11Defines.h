#pragma once

#if defined(DEBUG) | defined(_DEBUG)

#ifndef __MYE_DX11_HR_DEBUG

#define __MYE_DX11_HR_DEBUG(hr)\
		if(FAILED(hr))\
		{\
			LPTSTR output;\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,\
			nullptr,\
			hr,\
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
			(LPTSTR) &output,\
			0x0,\
			nullptr);\
			MessageBox(nullptr, output, "DirectX Error", MB_OK);\
		}
#endif

namespace mye
{
	namespace dx11
	{
		bool HRTestFailed(HRESULT hr);
	}
}

#ifndef __MYE_DX11_HR_TEST_FAILED(hr)
#define __MYE_DX11_HR_TEST_FAILED(hr) mye::dx11::HRTestFailed(hr)
#endif

#else

#ifndef __MYE_DX11_HR_DEBUG
#define __MYE_DX11_HR_DEBUG(x) { HRESULT __hr = x; }
#endif

#ifndef __MYE_DX11_HR_TEST_FAILED(hr)
#define __MYE_DX11_HR_TEST_FAILED(hr) FAILED(hr)
#endif

#endif

#ifndef __MYE_DX11_RELEASE_COM(x)

#define __MYE_DX11_RELEASE_COM(x) { x->Release(); x = nullptr; }
#define __MYE_DX11_RELEASE_COM_OPTIONAL(x) { if (x) __MYE_DX11_RELEASE_COM(x); }

#endif

#define __MYE_DX11_TEXTURE_SLOT_DIFFUSE             0
#define __MYE_DX11_TEXTURE_SLOT_NORMALMAP           1
#define __MYE_DX11_TEXTURE_SLOT_HEIGHTMAP           2
#define __MYE_DX11_TEXTURE_SLOT_SHADOWMAP           3
#define __MYE_DX11_TEXTURE_SLOT_RSMPOSITION         4
#define __MYE_DX11_TEXTURE_SLOT_RSMNORMAL           5
#define __MYE_DX11_TEXTURE_SLOT_RSMFLUX             6
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER0            10
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER1            11
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER2            12
#define __MYE_DX11_TEXTURE_SLOT_GBUFFER3            13
											        
#define __MYE_DX11_SAMPLER_SLOT_DIFFUSE             0
#define __MYE_DX11_SAMPLER_SLOT_NORMALMAP           1
#define __MYE_DX11_SAMPLER_SLOT_HEIGHTMAP           2
#define __MYE_DX11_SAMPLER_SLOT_SHADOWMAP           3
#define __MYE_DX11_SAMPLER_SLOT_SHADOWMAP_CMP       4

#define __MYE_DX11_BUFFER_SLOT_CAMERATRANSFORM      7
#define __MYE_DX11_BUFFER_SLOT_LIGHT                8
#define __MYE_DX11_BUFFER_SLOT_MATERIAL             9
#define __MYE_DX11_BUFFER_SLOT_RENDERCONFIGURATION  10
#define __MYE_DX11_BUFFER_SLOT_LIGHTSPACETRANSFORM  11
#define __MYE_DX11_BUFFER_SLOT_PSSMBUFFER           12