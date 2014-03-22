#pragma once

#include <mye/core/Resource.h>
#include <mye/core/String.h>

#include <mye/math/Math.h>

#include <luabind/luabind.hpp>
#include <luabind/value_wrapper.hpp>

#include <btBulletCollisionCommon.h>

#include <unordered_map>

namespace luabind
{

	/* mye::core::String */

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

	/* btVector3 */

	template <>
	struct default_converter<btVector3> :
		native_converter_base<btVector3>
	{

		inline static int compute_score(lua_State *L, int index)
		{
			return (object_cast_nothrow<mye::math::Vector3>(object(from_stack(L, index))) ? 0 : -1);
		}

		inline static btVector3 from(lua_State* L, int index)
		{
			mye::math::Vector3 v = object_cast<mye::math::Vector3>(object(from_stack(L, index)));
			return btVector3(v.x(), v.y(), v.z());
		}

		inline static void to(lua_State* L, const btVector3 &v)
		{
			luabind::object o(L, mye::math::Vector3(v.x(), v.y(), v.z()));
			o.push(L);
		}


	};

	template <>
	struct default_converter<btVector3 const>
		: default_converter<btVector3>
	{};

	template <>
	struct default_converter<btVector3 const&>
		: default_converter<btVector3>
	{};

	/* std::unordered_map<mye::core::String, mye::core::String> */

#define __UNORDEREDMAP_STRING_STRING std::unordered_map<mye::core::String, mye::core::String>

	template <>
	struct default_converter<__UNORDEREDMAP_STRING_STRING> :
		native_converter_base<__UNORDEREDMAP_STRING_STRING>
	{

		inline static int compute_score(lua_State *L, int index)
		{

			bool score = true;

			if (lua_type(L, index) == LUA_TTABLE)
			{
				
				lua_pushnil(L);

				while (score && lua_next(L, index) != 0)
				{

					if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TSTRING)
					{
						lua_pop(L, 2);
						score = false;
					}

					lua_pop(L, 1);

				}

			}
			else
			{
				score = false;
			}

			return score;

		}

		inline static __UNORDEREDMAP_STRING_STRING from(lua_State* L, int index)
		{
			
			__UNORDEREDMAP_STRING_STRING m;

			lua_pushnil(L);

			while (lua_next(L, index) != 0)
			{

				m.insert(std::pair<mye::core::String, mye::core::String>(lua_tostring(L, -2), lua_tostring(L, -1)));

				lua_pop(L, 1);

			}

			return m;

		}

		inline static void to(lua_State* L, const __UNORDEREDMAP_STRING_STRING &m)
		{
			
			lua_createtable(L, 0, m.size());

			for (auto pair : m)
			{

				lua_pushstring(L, pair.first.CString());
				lua_pushstring(L, pair.second.CString());

				lua_rawset(L, -3);

			}

		}


	};

	template <>
	struct default_converter<__UNORDEREDMAP_STRING_STRING const>
		: default_converter<__UNORDEREDMAP_STRING_STRING>
	{};

	template <>
	struct default_converter<__UNORDEREDMAP_STRING_STRING const&>
		: default_converter<__UNORDEREDMAP_STRING_STRING>
	{};

	/*template <>
	struct default_converter<mye::core::Resource::ParametersList>
		: default_converter<__UNORDEREDMAP_STRING_STRING>
	{};

	template <>
	struct default_converter<mye::core::Resource::ParametersList const>
		: default_converter<mye::core::Resource::ParametersList>
	{};

	template <>
	struct default_converter<mye::core::Resource::ParametersList const&>
		: default_converter<mye::core::Resource::ParametersList>
	{};*/

#undef __UNORDEREDMAP_STRING_STRING

}