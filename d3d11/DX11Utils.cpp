#include "DX11Utils.h"
#include "DX11Device.h"

#include <cassert>

#include <mye/win/Utils.h>

namespace mye
{
	namespace dx11
	{
		bool HRTestFailed(HRESULT hr)
		{

			if (FAILED(hr))
			{

				LPTSTR output;

				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,
					hr,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &output,
					0x0,
					nullptr);

				mye::win::ShowErrorBox(output);

				return true;

			}

			return false;

		}

		ID3D11SamplerState * CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, int maxAnisotropy, D3D11_COMPARISON_FUNC comparison)
		{

			D3D11_SAMPLER_DESC samplerDesc;

			samplerDesc.Filter         = filter;
			samplerDesc.AddressU       = addressMode;
			samplerDesc.AddressV       = addressMode;
			samplerDesc.AddressW       = addressMode;
			samplerDesc.MipLODBias     = 0.f;
			samplerDesc.MaxAnisotropy  = maxAnisotropy;
			samplerDesc.ComparisonFunc = comparison;
			samplerDesc.BorderColor[0] = 0;
			samplerDesc.BorderColor[1] = 0;
			samplerDesc.BorderColor[2] = 0;
			samplerDesc.BorderColor[3] = 0;
			samplerDesc.MinLOD         = - D3D11_FLOAT32_MAX;
			samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

			ID3D11SamplerState * sampler;

			if (__MYE_DX11_HR_TEST_FAILED(DX11Device::GetSingleton()->CreateSamplerState(&samplerDesc, &sampler)))
			{
				return nullptr;
			}
			else
			{
				return sampler;
			}

		}

	}
}