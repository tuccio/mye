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

			ScriptComponent(const std::string &name);
			~ScriptComponent(void);

			//const Script& Get(void);

		};

	}

}

