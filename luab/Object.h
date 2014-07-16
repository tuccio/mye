#pragma once

#include <lua.hpp>

namespace mye
{

	namespace luab
	{

		enum class ReferenceType
		{
			NONE,
			STACK
		};

		struct StackReference
		{
			lua_State *L;
			int        index;
		};

		class Object
		{

		public:

			Object(void);
			Object(StackReference stackReference);

			~Object(void);

			int GetLuaType(void);

		private:

			union
			{
				StackReference stack;
			} m_reference;

			ReferenceType m_referenceType;

		};

	}

}

