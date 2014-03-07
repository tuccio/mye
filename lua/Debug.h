#pragma once

#include <iostream>

#if _DEBUG

#define __SHOWLUASTACK(lstack)	\
	for (int i=0;i<lua_gettop(lstack)+1;i+=1)\
	{\
	if (lua_isnumber(lstack,i))\
		{\
		std::cout << i << ": number : " << lua_tonumber(lstack,i) << std::endl;\
		}\
		else if (lua_isstring(lstack,i))\
		{\
		std::cout << i << ": string : " << lua_tostring(lstack,i) << std::endl;\
		}\
		else if (lua_istable(lstack,i))\
		{\
		std::cout << i << ": table" << std::endl;\
		}\
		else if (lua_iscfunction(lstack,i))\
		{\
		std::cout << i << ": cfunction" << std::endl;\
		}\
		else if (lua_isfunction(lstack,i))\
		{\
		std::cout << i << ": function" << std::endl;\
		}\
		else if (lua_isboolean(lstack,i))\
		{\
		if (lua_toboolean(lstack,i)==true)\
		std::cout << i << ": boolean : true" << std::endl;\
			else\
			std::cout << i << ": boolean : false" << std::endl;\
		}\
		else if (lua_isuserdata(lstack,i))\
		{\
		std::cout << i << ": userdata" << std::endl;\
		}\
		else if (lua_isnil(lstack,i))\
		{\
		std::cout << i << ": nil" << std::endl;\
		}\
		else if (lua_islightuserdata(lstack,i))\
		{\
		std::cout << i << ": light userdata" << std::endl;\
		}\
	}

#else

#define __SHOWLUASTACK(lstack)

#endif