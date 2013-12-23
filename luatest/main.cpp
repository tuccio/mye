#include <mye/lua/Interpreter.h>

#include <iostream>

using namespace std;

#define VECTEST 1
#define GOTEST 2
#define GOCLASS 3

#define TEST 3

int main(int argc, char *argv[])
{	

	mye::lua::Interpreter lint;

	std::string testfile;

#if TEST == VECTEST
	testfile = "vectest.lua";
#elif TEST == GOTEST
	testfile = "gotest.lua";
#elif TEST == GOCLASS
	testfile = "goclass.lua";
#endif

	if (!lint.Run(testfile))
	{
		cout << "Error: " << lint.GetLastError() << endl;
		system("pause");
	}
	return 0;

}