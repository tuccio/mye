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

			DX11RasterizerState(const mye::core::RasterizerInfo & info);
			~DX11RasterizerState(void);

			void Use(void);

		private:

			ID3D11RasterizerState *m_rasterizerState;

		};

	}

}

