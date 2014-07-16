#include <iostream>
#include <string>

#include <functional>

#include <lua.hpp>

#include <mye/luab/luab.h>

struct A
{

	void HelloWorld(void)
	{
		std::cout << "Hello, world!" << std::endl;
	}

};

int main(int argc, char *argv[])
{

	lua_State *L = luaL_newstate();

	mye::luab::RegisterClass<A>("A").
		Define("HelloWorld", &A::HelloWorld);

	do 
	{

		std::string line;

		std::getline(std::cin, line);

		if (!line.empty())
		{

			luaL_dostring(L, line.c_str());

		}
		else
		{
			break;
		}


	} while (true);

	lua_close(L);

	return 0;

}