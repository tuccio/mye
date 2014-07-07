#pragma once

#include <fstream>

#if _DEBUG

#define __SHOWLUASTACK(lstack)	\
	{\
	static std::ofstream __f("lua_stack.txt");\
for (int i = 0; i < lua_gettop(lstack) + 1; i++)\
	{\
if (lua_isnumber(lstack, i))\
		{\
		__f << i << ": number : " << lua_tonumber(lstack, i) << std::endl; \
		}\
		else if (lua_isstring(lstack, i))\
		{\
		__f << i << ": string : " << lua_tostring(lstack, i) << std::endl; \
		}\
		else if (lua_istable(lstack, i))\
		{\
		__f << i << ": table" << std::endl; \
		}\
		else if (lua_iscfunction(lstack, i))\
		{\
		__f << i << ": cfunction" << std::endl; \
		}\
		else if (lua_isfunction(lstack, i))\
		{\
		__f << i << ": function" << std::endl; \
		}\
		else if (lua_isboolean(lstack, i))\
		{\
if (lua_toboolean(lstack, i) == true)\
__f << i << ": boolean : true" << std::endl; \
			else\
			__f << i << ": boolean : false" << std::endl; \
		}\
		else if (lua_isuserdata(lstack, i))\
		{\
		__f << i << ": userdata" << std::endl; \
		}\
		else if (lua_isnil(lstack, i))\
		{\
		__f << i << ": nil" << std::endl; \
		}\
		else if (lua_islightuserdata(lstack, i))\
		{\
		__f << i << ": light userdata" << std::endl; \
		}\
	}\
	}

#else

#define __SHOWLUASTACK(lstack)

#endif