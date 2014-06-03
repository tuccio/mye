#pragma once

#include "DX11Device.h"

#include <mye/core/RasterizerInfo.h>

namespace mye
{

	namespace dx11
	{

		class DX11RasterizerState
		{

		public:

			DX11RasterizerState(DX11Device &device, const mye::core::RasterizerInfo &info);
			~DX11RasterizerState(void);

			void Use(void);

		private:

			DX11Device &m_device;
			ID3D11RasterizerState *m_rasterizerState;

		};

	}

}

