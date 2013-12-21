#include <mye/lua/Interpreter.h>

#include <iostream>

using namespace std;

#define VECTEST 0
#define GOTEST 1

int main(int argc, char *argv[])
{	

	mye::lua::Interpreter lint;

#if VECTEST

	if (!lint.Run("vectest.lua"))
	{
		cout << "Error: " << lint.GetLastError() << endl;
		system("pause");
	}

#endif

#if GOTEST

	if (!lint.Run("gotest.lua"))
	{
		cout << "Error: " << lint.GetLastError() << endl;
		system("pause");
	}

#endif

	return 0;

}