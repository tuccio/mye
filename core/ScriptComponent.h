#pragma once

#include "Component.h"
#include "Script.h"

namespace mye
{

	namespace core
	{

		class ScriptComponent :
			public Component
		{

		public:

			ScriptComponent(void);
			~ScriptComponent(void);

			//const Script& Get(void);

		};

	}

}

