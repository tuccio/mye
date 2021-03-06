#pragma once

#include <d3d11.h>

#include <mye/math/Math.h>

namespace mye
{

	namespace dx11
	{

		class DX11RenderTarget
		{

		public:

			DX11RenderTarget(void) : m_renderTargetView(nullptr) { }

			inline ID3D11RenderTargetView * GetRenderTargetView(void) const
			{
				return m_renderTargetView;
			}

			void ClearRenderTarget(const mye::math::Vector4f & color)
			{
				DX11Device::GetSingleton().GetImmediateContext()->ClearRenderTargetView(m_renderTargetView, color.Data());
			}

		protected:

			ID3D11RenderTargetView * m_renderTargetView;


		};

	}
}