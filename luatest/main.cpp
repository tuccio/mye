#include <mye/lua/Interpreter.h>

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{	

	mye::lua::Interpreter lint;

	if (!lint.Run("test.lua"))
	{
		cout << "Error: " << lint.GetLastError() << endl;
		system("pause");
	}

	return 0;

}