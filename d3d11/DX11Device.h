#pragma once

#include <d3d11.h>

#include <mye/core/ColorRGBA.h>

namespace mye
{

	namespace dx11
	{

		class DX11Device
		{

		public:

			DX11Device(void);
			~DX11Device(void);

			inline ID3D11Device* GetDevice(void) const;
			inline ID3D11DeviceContext* GetImmediateContext(void) const;

		private:

			ID3D11Device *m_device;
			ID3D11DeviceContext *m_dImmediateContext;

		};

		ID3D11Device* DX11Device::GetDevice(void) const
		{
			return m_device;
		}

		ID3D11DeviceContext* DX11Device::GetImmediateContext(void) const
		{
			return m_dImmediateContext;
		}

	}

}
