#pragma once

#include <d3d11.h>

namespace mye
{

	namespace dx11
	{

		class DX11VertexBuffer;

		class DX11Device
		{

		public:

			DX11Device(void);
			~DX11Device(void);

			inline ID3D11Device* GetDevice(void) const;
			inline ID3D11DeviceContext* GetImmediateContext(void) const;

			inline ID3D11Device* operator-> (void);

			bool Create(void);
			void Destroy(void);

			bool Exists(void) const;

			void SetBlending(bool enable);
			void SetDepthTest(bool enable);

		private:

			ID3D11Device *m_device;
			ID3D11DeviceContext *m_dImmediateContext;

			ID3D11BlendState *m_blendOff;
			ID3D11BlendState *m_blendOn;

			ID3D11DepthStencilState *m_depthTestOff;
			ID3D11DepthStencilState *m_depthTestOn;

		};

		ID3D11Device* DX11Device::operator-> (void)
		{
			return m_device;
		}

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
