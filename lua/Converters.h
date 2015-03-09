#pragma once

#include <mye/core/Parameters.h>
#include <mye/core/String.h>

/* Resources */

#include <mye/core/BulletCollisionShape.h>
#include <mye/core/Font.h>
#include <mye/core/Game.h>
#include <mye/core/GameObjectsModule.h>
#include <mye/core/Light.h>
#include <mye/core/Resource.h>

#include <mye/math/Math.h>

#include "BehaviourScript.h"

#include <btBulletCollisionCommon.h>

#include <unordered_map>

#include <lua++11/lua++11.h>

namespace luapp11
{

	/* String */

	template <>
	struct LuaToCppConverter<mye::core::String>
	{

		static bool IsConvertible(lua_State * L, int index)
		{
			return lua_type(L, index) == LUA_TSTRING;
		}

		static mye::core::String Apply(lua_State * L, int index)
		{
			const char * s = lua_tostring(L, index);
			return mye::core::String(s);
		}

	};

	template <>
	struct LuaToCppConverter<const mye::core::String> :
			LuaToCppConverter<mye::core::String> { };

	template <>
	struct LuaToCppConverter<const mye::core::String &> :
			LuaToCppConverter<mye::core::String> { };

	template <>
	struct CppToLuaConverter<mye::core::String>
	{

		static void Apply(lua_State * L, const mye::core::String * s)
		{
			lua_pushstring(L, s->CString());
		}

		static void Apply(lua_State * L, const mye::core::String & s)
		{
			lua_pushstring(L, s.CString());
		}

	};

	template <>
	struct CppToLuaConverter<const mye::core::String> :
		CppToLuaConverter<mye::core::String> { };

	template <>
	struct CppToLuaConverter<const mye::core::String &> :
		CppToLuaConverter<mye::core::String> { };

	/* Bullet structures */

	template <>
	struct LuaToCppConverter<btVector3>
	{

		inline static bool IsConvertible(lua_State * L, int index)
		{
			return LuaToCppConverter<mye::math::Vector3>::IsConvertible(L, index);
		}

		inline static btVector3 Apply(lua_State * L, int index)
		{
			mye::math::Vector3 v = LuaToCppConverter<mye::math::Vector3>::Apply(L, index);
			return btVector3(v.x(), v.y(), v.z());
		}

	};

	template <>
	struct CppToLuaConverter<btVector3>
	{

		static void Apply(lua_State * L, const mye::math::Vector3 & v)
		{
			CppToLuaConverter::Apply(L, v);
		}

	};


	template <>
	struct LuaBehaviourConverter<mye::core::GameObjectHandle>
	{

		static LuaBehaviour GetBehaviour(const mye::core::GameObjectHandle * hObj)
		{

			mye::core::GameObject * go = mye::core::Game::GetSingleton().GetGameObjectsModule()->Get(*hObj);

			if (go)
			{

				mye::core::BehaviourComponent * bc = static_cast<mye::core::BehaviourComponent*>(go->GetBehaviourComponent());


				if (bc)
				{
					auto script = mye::core::Resource::StaticCast<mye::lua::BehaviourScript>(bc->GetScript());
					return script->GetBehaviour();
				}

			}

			return LuaBehaviour();

		}

	};

}


//
//namespace luabind
//{
//
//	/* mye::core::String */
//
//	template <>
//	struct default_converter<mye::core::String> :
//		native_converter_base<mye::core::String>
//	{
//
//		inline static int compute_score(lua_State* L, int index)
//		{
//			return lua_type(L, index) == LUA_TSTRING ? 0 : -1;
//		}
//
//		inline static mye::core::String from(lua_State* L, int index)
//		{
//			return mye::core::String(lua_tostring(L, index));
//		}
//
//		inline static void to(lua_State* L, const mye::core::String &s)
//		{
//			lua_pushstring(L, s.CString());
//		}
//
//	};
//
//	template <>
//	struct default_converter<mye::core::String const>
//		: default_converter<mye::core::String>
//	{};
//
//	template <>
//	struct default_converter<mye::core::String const&>
//		: default_converter<mye::core::String>
//	{};
//
//	/* btVector3 */
//
//	template <>
//	struct default_converter<btVector3> :
//		native_converter_base<btVector3>
//	{
//
//		inline static int compute_score(lua_State *L, int index)
//		{
//			return (object_cast_nothrow<mye::math::Vector3>(object(from_stack(L, index))) ? 0 : -1);
//		}
//
//		inline static btVector3 from(lua_State* L, int index)
//		{
//			mye::math::Vector3 v = object_cast<mye::math::Vector3>(object(from_stack(L, index)));
//			return btVector3(v.x(), v.y(), v.z());
//		}
//
//		inline static void to(lua_State* L, const btVector3 &v)
//		{
//			luabind::object o(L, mye::math::Vector3(v.x(), v.y(), v.z()));
//			o.push(L);
//		}
//
//
//	};
//
//	template <>
//	struct default_converter<btVector3 const>
//		: default_converter<btVector3>
//	{};
//
//	template <>
//	struct default_converter<btVector3 const&>
//		: default_converter<btVector3>
//	{};
//
//	/* btQuaternion */
//
//	template <>
//	struct default_converter<btQuaternion> :
//		native_converter_base<btQuaternion>
//	{
//
//		inline static int compute_score(lua_State *L, int index)
//		{
//			return (object_cast_nothrow<mye::math::Quaternion>(object(from_stack(L, index))) ? 0 : -1);
//		}
//
//		inline static btQuaternion from(lua_State* L, int index)
//		{
//			mye::math::Quaternion q = object_cast<mye::math::Quaternion>(object(from_stack(L, index)));
//			return btQuaternion(q.x(), q.y(), q.z(), q.w());
//		}
//
//		inline static void to(lua_State* L, const btVector3 &v)
//		{
//			luabind::object o(L, mye::math::Quaternion(v.w(), v.x(), v.y(), v.z()));
//			o.push(L);
//		}
//
//
//	};
//
//	template <>
//	struct default_converter<btQuaternion const>
//		: default_converter<btQuaternion>
//	{};
//
//	template <>
//	struct default_converter<btQuaternion const&>
//		: default_converter<btQuaternion>
//	{};
//
//	/* std::unordered_map<mye::core::String, mye::core::String> */
//
//#define __UNORDEREDMAP_STRING_STRING std::unordered_map<mye::core::String, mye::core::String>
//
//	template <>
//	struct default_converter<__UNORDEREDMAP_STRING_STRING> :
//		native_converter_base<__UNORDEREDMAP_STRING_STRING>
//	{
//
//		inline static int compute_score(lua_State *L, int index)
//		{
//
//			bool score = true;
//
//			if (lua_type(L, index) == LUA_TTABLE)
//			{
//				
//				lua_pushnil(L);
//
//				while (score && lua_next(L, index) != 0)
//				{
//
//					if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TSTRING)
//					{
//						lua_pop(L, 2);
//						score = false;
//					}
//
//					lua_pop(L, 1);
//
//				}
//
//			}
//			else
//			{
//				score = false;
//			}
//
//			return score;
//
//		}
//
//		inline static __UNORDEREDMAP_STRING_STRING from(lua_State* L, int index)
//		{
//			
//			__UNORDEREDMAP_STRING_STRING m;
//
//			if (lua_type(L, index) == LUA_TTABLE)
//			{
//
//				lua_pushnil(L);
//
//				while (lua_next(L, index) != 0)
//				{
//
//					m.insert(std::pair<mye::core::String, mye::core::String>(lua_tostring(L, -2), lua_tostring(L, -1)));
//
//					lua_pop(L, 1);
//
//				}
//
//			}
//
//			return m;
//
//		}
//
//		inline static void to(lua_State* L, const __UNORDEREDMAP_STRING_STRING &m)
//		{
//			
//			lua_createtable(L, 0, m.size());
//
//			for (auto pair : m)
//			{
//
//				lua_pushstring(L, pair.first.CString());
//				lua_pushstring(L, pair.second.CString());
//
//				lua_rawset(L, -3);
//
//			}
//
//		}
//
//
//	};
//
//	template <>
//	struct default_converter<__UNORDEREDMAP_STRING_STRING const>
//		: default_converter<__UNORDEREDMAP_STRING_STRING>
//	{};
//
//	template <>
//	struct default_converter<__UNORDEREDMAP_STRING_STRING const&>
//		: default_converter<__UNORDEREDMAP_STRING_STRING>
//	{};
//
//	/*template <>
//	struct default_converter<mye::core::Parameters>
//		: default_converter<__UNORDEREDMAP_STRING_STRING>
//	{};
//
//	template <>
//	struct default_converter<mye::core::Parameters const>
//		: default_converter<mye::core::Parameters>
//	{};
//
//	template <>
//	struct default_converter<mye::core::Parameters const&>
//		: default_converter<mye::core::Parameters>
//	{};*/
//
//#undef __UNORDEREDMAP_STRING_STRING
//
//	template <>
//	struct default_converter<mye::core::Parameters> :
//		native_converter_base<mye::core::Parameters>
//	{
//
//		inline static int compute_score(lua_State *L, int index)
//		{
//
//			bool score = true;
//
//			if (lua_type(L, index) == LUA_TTABLE)
//			{
//
//				lua_pushnil(L);
//
//				while (score && lua_next(L, index) != 0)
//				{
//
//					if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TSTRING)
//					{
//						lua_pop(L, 2);
//						score = false;
//					}
//
//					lua_pop(L, 1);
//
//				}
//
//			}
//			else
//			{
//				score = false;
//			}
//
//			return score;
//
//		}
//
//		inline static mye::core::Parameters from(lua_State* L, int index)
//		{
//
//			int top = lua_gettop(L);
//
//			mye::core::Parameters m;
//
//			lua_pushnil(L);
//
//			while (lua_next(L, index) != 0)
//			{
//
//				m.Add(lua_tostring(L, -2), lua_tostring(L, -1));
//
//				lua_pop(L, 1);
//
//			}
//
//			//int newtop = lua_gettop(L);
//
//			return m;
//
//		}
//
//		inline static void to(lua_State* L, const mye::core::Parameters &m)
//		{
//
//			lua_createtable(L, 0, m.Size());
//
//			for (auto pair : m)
//			{
//
//				lua_pushstring(L, pair.first.CString());
//				lua_pushstring(L, pair.second.CString());
//
//				lua_rawset(L, -3);
//
//			}
//
//		}
//
//
//	};
//
//	template <>
//	struct default_converter<mye::core::Parameters const>
//		: default_converter<mye::core::Parameters>
//	{};
//
//	template <>
//	struct default_converter<mye::core::Parameters const&>
//		: default_converter<mye::core::Parameters>
//	{};
//
//}
//
//#define __MYE_DEFINE_RESOURCE_LUA_CONVERTER(__TYPE)\
//namespace luabind\
//{\
//	template <>\
//	struct default_converter<std::shared_ptr<__TYPE>> :\
//		native_converter_base<std::shared_ptr<__TYPE>>\
//	{\
//		inline static int compute_score(lua_State *L, int index)\
//		{\
//			return (object_cast_nothrow<std::shared_ptr<mye::core::Resource>>(object(from_stack(L, index))) ? 0 : -1);\
//		}\
//		inline static std::shared_ptr<__TYPE> from(lua_State* L, int index)\
//		{\
//			auto r = object_cast<std::shared_ptr<mye::core::Resource>>(object(from_stack(L, index)));\
//			return boost::static_pointer_cast<__TYPE>(r);\
//		}\
//		inline static void to(lua_State* L, const std::shared_ptr<__TYPE> &p)\
//		{\
//			luabind::object o(L, p);\
//			o.push(L);\
//		}\
//	};\
//	template <>\
//	struct default_converter<__TYPE const>\
//		: default_converter<__TYPE>\
//	{};\
//	template <>\
//	struct default_converter<__TYPE const&>\
//	: default_converter<__TYPE>\
//	{};\
//}
//
///*__MYE_DEFINE_RESOURCE_LUA_CONVERTER(mye::core::BulletCollisionShape)
//__MYE_DEFINE_RESOURCE_LUA_CONVERTER(mye::core::Font)*/
//
//#define __MYE_LUA_ADAPT_ENUM_CLASS(__ENUM) namespace luabind { \
//		template <> struct default_converter<__ENUM> : native_converter_base<__ENUM>{ \
//		inline static int compute_score(lua_State *L, int index) { return lua_isnumber(L, index); } \
//		inline static __ENUM from(lua_State* L, int index) { return static_cast<__ENUM>(lua_tointeger(L, index)); } \
//		inline static void to(lua_State* L, const __ENUM &vs) { lua_pushinteger(L, static_cast<int>(vs)); } }; \
//		template <> struct default_converter<__ENUM const>  : default_converter<__ENUM>{}; \
//		template <> struct default_converter<__ENUM const&> : default_converter<__ENUM>{}; }
//
//__MYE_LUA_ADAPT_ENUM_CLASS(mye::math::VolumeSide)
//__MYE_LUA_ADAPT_ENUM_CLASS(mye::core::LightType)