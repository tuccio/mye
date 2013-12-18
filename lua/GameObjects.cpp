#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/return_reference_to_policy.hpp>

#include <mye/core/GameObject.h>
#include <mye/core/Game.h>

namespace mye
{

	namespace lua
	{

		class Wrapper
		{

		public:

			Wrapper(int i)
			{
				_x.i = i;
			}

			Wrapper(float f)
			{
				_x.f = f;
			}

		private:

			union Types
			{
				int i;
				float f;
			} _x;

		};

		int RegisterGameObjects(lua_State *L)
		{

			using namespace mye::core;
			using namespace luabind;

			open(L);

			module(L)
			[
				def("CreateGameObject", &Game::CreateGameObject)
			];

			module(L)
			[
				class_<GameObjectHandle>("GameObjectHandle").
					def("Exists", Game::IsGameObject).
					def("Destroy", &Game::DestroyGameObject)
			];

			return 0;

		}

	}

}