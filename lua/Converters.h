#pragma once

#include <mye/core/String.h>
#include <luabind/luabind.hpp>

namespace luabind
{

	template <>
	struct default_converter<mye::core::String> :
		native_converter_base<mye::core::String>
	{

		inline static int compute_score(lua_State* L, int index)
		{
			return lua_type(L, index) == LUA_TSTRING ? 0 : -1;
		}

		inline static mye::core::String from(lua_State* L, int index)
		{
			return mye::core::String(lua_tostring(L, index));
		}

		inline static void to(lua_State* L, const mye::core::String &s)
		{
			lua_pushstring(L, s.CString());
		}

	};

	template <>
	struct default_converter<mye::core::String const>
		: default_converter<mye::core::String>
	{};

	template <>
	struct default_converter<mye::core::String const&>
		: default_converter<mye::core::String>
	{};

}