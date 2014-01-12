#pragma once

#include "LuaModule.h"

namespace mye
{

	namespace lua
	{

		void BindMath(lua_State *L);

		template <typename T>
		class AlignmentWrapper :
			public T
		{

		public:

			AlignmentWrapper() {}
			AlignmentWrapper(const T& x) : T(x) { }

		};

	}

}