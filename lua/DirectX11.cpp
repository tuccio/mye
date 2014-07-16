#include "DirectX11.h"

#include "Types.h"
#include "Converters.h"
#include "Alignment.h"

#include <mye/win/Window.h>

#include <mye/d3d11/DX11Module.h>
#include <mye/d3d11/DX11Font.h>

#include <luabind/luabind.hpp>

using namespace mye::core;
using namespace mye::win;
using namespace mye::dx11;
using namespace luabind;

namespace mye
{

	namespace lua
	{

		void BindDirectX11(lua_State *L)
		{

			module(L)
			[

				class_<DX11Module, GraphicsModule>(MYE_LUA_DX11MODULE),

				class_<DX11Font, Font>(MYE_LUA_DX11FONT)

			];

		}

	}

}