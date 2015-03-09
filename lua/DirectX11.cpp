#include "DirectX11.h"

#include "Types.h"
#include "Converters.h"

#include <mye/win/Window.h>

#include <mye/d3d11/DX11Module.h>
#include <mye/d3d11/DX11Font.h>

using namespace mye::core;
using namespace mye::win;
using namespace mye::dx11;

namespace mye
{

	namespace lua
	{

		void BindDirectX11(luapp11::State state)
		{

			using namespace luapp11;

			state
			[

				Class<DX11Module, GraphicsModule>(MYE_LUA_DX11MODULE),

				Class<DX11Font, Font>(MYE_LUA_DX11FONT)

			];

		}

	}

}