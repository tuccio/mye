#pragma once

#include <mye/core/Game.h>

#include <lua++11/lua++11.h>

namespace mye
{

	namespace lua
	{

		void BindGame(luapp11::State state);
		void BindWindow(luapp11::State state);

	}

}