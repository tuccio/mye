#pragma once

#include <d3d11.h>

#include <type_traits>
#include <vector>

namespace mye
{

	namespace dx11
	{

		template <typename T>
		struct RenderTargetGrabber
		{
			static inline ID3D11RenderTargetView * GetRenderTargetView(T & renderTarget)
			{
				return renderTarget.GetRenderTargetView();
			}
		};

		template <>
		struct RenderTargetGrabber<ID3D11RenderTargetView *>
		{
			static inline ID3D11RenderTargetView * GetRenderTargetView(ID3D11RenderTargetView * rtv)
			{
				return rtv;
			}
		};

		class DX11VertexBuffer;

		enum class DX11DepthTest
		{
			ON, OFF, LOOKUP
		};

		enum class DX11BlendState
		{
			ON, OFF, ACCUMULATE
		};

		enum class MSAA
		{
			MSAA_OFF = 0,
			MSAA_4X = 4,
			MSAA_8X = 8,
			MSAA_16X = 16
		};

		class DX11Device
		{

		public:

			DX11Device(void);
			~DX11Device(void);

			inline ID3D11Device        * GetDevice(void) const;
			inline ID3D11DeviceContext * GetImmediateContext(void) const;

			inline ID3D11Device * operator-> (void);

			bool Create(void);
			void Destroy(void);

			bool Exists(void) const;

			void SetBlending(bool enable);
			void SetDepthTest(DX11DepthTest test);

			DXGI_SAMPLE_DESC GetMSAASampleDesc(MSAA msaa, DXGI_FORMAT format);

			std::vector<D3D11_VIEWPORT>	GetViewports(void);

			void SetViewports(const D3D11_VIEWPORT * viewports, unsigned int numViewports);

		private:

			ID3D11Device            * m_device;
			ID3D11DeviceContext     * m_dImmediateContext;

			ID3D11BlendState        * m_blendOff;
			ID3D11BlendState        * m_blendOn;

			ID3D11DepthStencilState * m_depthTestOff;
			ID3D11DepthStencilState * m_depthTestOn;
			ID3D11DepthStencilState * m_depthTestLookup;

		};

		inline ID3D11Device * DX11Device::operator-> (void)
		{
			return m_device;
		}

		inline ID3D11Device * DX11Device::GetDevice(void) const
		{
			return m_device;
		}

		inline ID3D11DeviceContext * DX11Device::GetImmediateContext(void) const
		{
			return m_dImmediateContext;
		}

	}

}
