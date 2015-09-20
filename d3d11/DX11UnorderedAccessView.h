#pragma once

#include <d3d11.h>

#include "DX11Utils.h"
#include "DX11Device.h"

namespace mye
{

	namespace dx11
	{

		class DX11UnorderedAccessView
		{

		public:

			DX11UnorderedAccessView(void) : m_unorderedAccessView(nullptr) { }

			inline ID3D11UnorderedAccessView * GetUnorrderedAccessView(void) const
			{
				return m_unorderedAccessView;
			}

			void BindUAV(int slot)
			{
				DX11Device::GetSingleton().GetImmediateContext()->CSSetUnorderedAccessViews(slot, 1, &m_unorderedAccessView, nullptr);
				m_lastBoundSlot = slot;
			}

			void UnbindUAV(void)
			{
				DX11Device::GetSingleton().GetImmediateContext()->CSSetUnorderedAccessViews(m_lastBoundSlot, 1, nullptr, nullptr);
			}

		protected:

			ID3D11UnorderedAccessView * m_unorderedAccessView;

		private:

			int                         m_lastBoundSlot;

		};

	}
}