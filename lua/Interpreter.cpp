#include "Interpreter.h"

#include "Vector.h"
#include "GameObjects.h"

using namespace mye::lua;
using namespace std;

Interpreter::Interpreter(void)
{


	_L = luaL_newstate();

	luaL_openlibs(_L);

	RegisterVector(_L);

 	RegisterGameObjects(_L);

}


Interpreter::~Interpreter(void)
{
	lua_close(_L);
}

bool Interpreter::Run(const string &file)
{

	if (luaL_loadfile(_L, file.c_str()) ||
		lua_pcall(_L, 0, 0, 0))
	{
		_lastError = lua_tostring(_L, -1);
		return false;
	}

	return true;

}

string Interpreter::GetLastError(void)
{
	return _lastError;
}

