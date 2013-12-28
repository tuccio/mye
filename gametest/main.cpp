#include <mye/lua/Interpreter.h>
#include <mye/win/WinGame.h>

#include <iostream>

using namespace std;
using namespace mye::core;
using namespace mye::lua;
using namespace mye::win;

#define VECTEST 1
#define GOTEST 2
#define GOCLASS 3
#define WINDOWTEST 4

#define TEST 4

int main(int argc, char *argv[])
{	

	InputModule input;
	GameObjectsModule gameobjects;
	SceneModule scene;
	GraphicsModule graphics;
	AudioModule audio;

	WinGame game(&input,
		&gameobjects,
		&scene,
		&graphics,
		&audio);

	game.Run();

	return 0;

}