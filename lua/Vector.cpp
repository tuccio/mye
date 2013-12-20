#include "Vector.h"
#include "Utils.h"
#include "Types.h"

#include <Eigen/Eigen>

#include <string>
#include <iostream>
#include <sstream>

namespace mye
{
	
	namespace lua
	{

		/* Constructor */

		int __vec3_new(lua_State *L);

		/* Metamethods */

		int __vec3_index(lua_State *L);
		int __vec3_newindex(lua_State *L);
		int __vec3_proxy(lua_State *L);

		/* Comparison operators */

		int __vec3_eq(lua_State *L);

		/* Arithmetic operators */

		int __vec3_add(lua_State *L);
		int __vec3_sub(lua_State *L);
		int __vec3_mul(lua_State *L);
		int __vec3_div(lua_State *L);

		int __vec3_length(lua_State *L);
		int __vec3_dot(lua_State *L);
		int __vec3_cross(lua_State *L);

		/* Output operations */

		int __vec3_tostring(lua_State *L);

		void RegisterVector(lua_State *L)
		{

			// Create a proxy table

			lua_newtable(L);

			// Create a metatable

			lua_newtable(L);

			lua_newtable(L);
			lua_setfield(L, -2, "__metatable");

			lua_pushcfunction(L, __mye_donothing);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, __vec3_new);
			lua_setfield(L, -2, "__call");

			lua_pushcfunction(L, __mye_donothing);
			lua_setfield(L, -2, "__newindex");

			lua_setmetatable(L, -2);

			lua_setglobal(L, MYE_LUA_VEC3);	

		}
		
		/* Constructor */

		int __vec3_new(lua_State *L)
		{

			int nargs = lua_gettop(L);
			bool error = false;

			if (nargs == 1)
			{
				Eigen::Vector3f v(0, 0, 0);
				void *d = lua_newuserdata(L, sizeof(Eigen::Vector3f));
				memcpy(d, &v, sizeof(Eigen::Vector3f));
			}
			else if (nargs == 2)
			{

				int type = lua_type(L, 2);

				if (type == LUA_TNUMBER)
				{
					float initvalue = lua_tonumber(L, 2);
					Eigen::Vector3f v(initvalue, initvalue, initvalue);
					void *d = lua_newuserdata(L, sizeof(Eigen::Vector3f));
					memcpy(d, &v, sizeof(Eigen::Vector3f));
				}
				else if (type == LUA_TUSERDATA)
				{

					if (luaL_checkudata(L, 2, MYE_LUA_VEC3))
					{
						lua_newuserdata(L, sizeof(Eigen::Vector3f));
						lua_copy(L, 2, -1);
					}
					else
					{
						error = true;
					}
					
				}

			}
			else if (nargs == 4)
			{
				Eigen::Vector3f v(lua_tonumber(L, 2),
					lua_tonumber(L, 3),
					lua_tonumber(L, 4));
				void *d = lua_newuserdata(L, sizeof(Eigen::Vector3f));
				memcpy(d, &v, sizeof(Eigen::Vector3f));
			}
			else
			{
				error = true;
			}

			if (error)
			{
				luaL_error(L, "vec3: Invalid constructor");
				return 0;
			}

			// Create the metatable			

			if (luaL_newmetatable(L, MYE_LUA_VEC3))
			{

				lua_pushcfunction(L, __vec3_index);
				lua_setfield(L, -2, "__index");

				lua_pushcfunction(L, __vec3_newindex);
				lua_setfield(L, -2, "__newindex");

				lua_pushcfunction(L, __vec3_eq);
				lua_setfield(L, -2, "__eq");

				lua_pushcfunction(L, __vec3_add);
				lua_setfield(L, -2, "__add");

				lua_pushcfunction(L, __vec3_sub);
				lua_setfield(L, -2, "__sub");

				lua_pushcfunction(L, __vec3_mul);
				lua_setfield(L, -2, "__mul");

				lua_pushcfunction(L, __vec3_div);
				lua_setfield(L, -2, "__div");

				lua_pushcfunction(L, __vec3_tostring);
				lua_setfield(L, -2, "__tostring");

			}

			lua_setmetatable(L, -2);

			return 1;

		}

		/* Metamethods */

		int __vec3_index(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 2)
			{
				luaL_error(L, "vec3: Assignment argument count error");
				return 0;
			}

			void *d = luaL_checkudata(L, 1, MYE_LUA_VEC3);

			if (!d)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f &v = * static_cast<Eigen::Vector3f*>(d);

			const char *field = lua_tostring(L, 2);

			if (!strcmp(field, "x"))
			{
				lua_pushnumber(L, v.x());
				return 1;
			}
			else if (!strcmp(field, "y"))
			{
				lua_pushnumber(L, v.y());
				return 1;
			}
			else if (!strcmp(field, "z"))
			{
				lua_pushnumber(L, v.z());
				return 1;
			}

			luaL_error(L, (std::string("vec3: Unknown field ") + std::string(field)).c_str());

			return 0;

		}

		int __vec3_newindex(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 3)
			{
				luaL_error(L, "vec3: Assignment argument count error");
				return 0;
			}

			void *d = luaL_checkudata(L, 1, MYE_LUA_VEC3);

			if (!d)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f &v = * static_cast<Eigen::Vector3f*>(d);

			const char *field = lua_tostring(L, 2);

			if (!strcmp(field, "x"))
			{
				v.x() = lua_tonumber(L, 3);
				return 0;
			}
			else if (!strcmp(field, "y"))
			{
				v.y() = lua_tonumber(L, 3);
				return 0;
			}
			else if (!strcmp(field, "z"))
			{
				v.z() = lua_tonumber(L, 3);
				return 0;
			}
			
			luaL_error(L, (std::string("vec3: Unknown field ") + std::string(field)).c_str());

			return 0;

		}

		int __vec3_proxy(lua_State *L)
		{

			return 0;

		}

		/* Comparison operators */

		int __vec3_eq(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 2)
			{
				luaL_error(L, "vec3: Compare argument count error");
				return 0;
			}

			void *d1 = luaL_checkudata(L, 1, MYE_LUA_VEC3);
			void *d2 = luaL_checkudata(L, 2, MYE_LUA_VEC3);

			if (d1 == NULL || d2 == NULL)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f *v1 = (Eigen::Vector3f*) d1;
			Eigen::Vector3f *v2 = (Eigen::Vector3f*) d2;

			lua_pushboolean(L, *v1 == *v2);

			return 1;

		}

		/* Arithmetic operators */

		int __vec3_add(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 2)
			{
				luaL_error(L, "vec3: Compare argument count error");
				return 0;
			}

			void *d1 = luaL_checkudata(L, 1, MYE_LUA_VEC3);
			void *d2 = luaL_checkudata(L, 2, MYE_LUA_VEC3);

			if (d1 == NULL || d2 == NULL)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f *v1 = (Eigen::Vector3f*) d1;
			Eigen::Vector3f *v2 = (Eigen::Vector3f*) d2;

			Eigen::Vector3f v(*v1 + *v2);

			lua_settop(L, 0);
			
			lua_getglobal(L, MYE_LUA_VEC3);
			lua_pushnumber(L, v.x());
			lua_pushnumber(L, v.y());
			lua_pushnumber(L, v.z());

			__vec3_new(L);

			return 1;

		}

		int __vec3_sub(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 2)
			{
				luaL_error(L, "vec3: Compare argument count error");
				return 0;
			}

			void *d1 = luaL_checkudata(L, 1, MYE_LUA_VEC3);
			void *d2 = luaL_checkudata(L, 2, MYE_LUA_VEC3);

			if (d1 == NULL || d2 == NULL)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f *v1 = (Eigen::Vector3f*) d1;
			Eigen::Vector3f *v2 = (Eigen::Vector3f*) d2;

			Eigen::Vector3f v(*v1 - *v2);

			lua_settop(L, 0);

			lua_getglobal(L, MYE_LUA_VEC3);
			lua_pushnumber(L, v.x());
			lua_pushnumber(L, v.y());
			lua_pushnumber(L, v.z());

			__vec3_new(L);

			return 1;

		}

		int __vec3_mul(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 2)
			{
				luaL_error(L, "vec3: Compare argument count error");
				return 0;
			}

			void *d1 = luaL_checkudata(L, 1, MYE_LUA_VEC3);
			void *d2 = luaL_checkudata(L, 2, MYE_LUA_VEC3);

			if (d1 == NULL || d2 == NULL)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f *v1 = (Eigen::Vector3f*) d1;
			Eigen::Vector3f *v2 = (Eigen::Vector3f*) d2;

			Eigen::Vector3f v(v1->cwiseProduct(*v2));

			lua_settop(L, 0);

			lua_getglobal(L, MYE_LUA_VEC3);
			lua_pushnumber(L, v.x());
			lua_pushnumber(L, v.y());
			lua_pushnumber(L, v.z());

			__vec3_new(L);

			return 1;

		}

		int __vec3_div(lua_State *L)
		{

			int nargs = lua_gettop(L);

			if (nargs != 2)
			{
				luaL_error(L, "vec3: Compare argument count error");
				return 0;
			}

			void *d1 = luaL_checkudata(L, 1, MYE_LUA_VEC3);
			void *d2 = luaL_checkudata(L, 2, MYE_LUA_VEC3);

			if (d1 == NULL || d2 == NULL)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			Eigen::Vector3f *v1 = (Eigen::Vector3f*) d1;
			Eigen::Vector3f *v2 = (Eigen::Vector3f*) d2;

			Eigen::Vector3f v(v1->cwiseProduct(v2->cwiseInverse()));

			lua_settop(L, 0);

			lua_getglobal(L, MYE_LUA_VEC3);
			lua_pushnumber(L, v.x());
			lua_pushnumber(L, v.y());
			lua_pushnumber(L, v.z());

			__vec3_new(L);

			return 1;

		}

		/* Output operations */

		int __vec3_tostring(lua_State *L)
		{

			void *d = luaL_checkudata(L, 1, MYE_LUA_VEC3);

			if (!d)
			{
				luaL_error(L, "vec3: Argument error");
				return 0;
			}

			static Eigen::IOFormat fmt(
				Eigen::StreamPrecision,
				Eigen::DontAlignCols,
				", ",
				", ",
				"",
				"",
				"[",
				"]");

			std::stringstream ss;

			Eigen::Vector3f &v = * static_cast<Eigen::Vector3f*>(d);

			ss << v.format(fmt);
			lua_pushstring(L, ss.str().c_str());
			return 1;

		}

	}

}