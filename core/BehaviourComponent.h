#pragma once

#include "Component.h"
#include "Script.h"
#include "Time.h"

namespace mye
{

	namespace core
	{

		class BehaviourComponent :
			public Component
		{

		public:

			BehaviourComponent(void);
			BehaviourComponent(const ScriptPointer & script);

			~BehaviourComponent(void);

			inline ScriptPointer GetScript(void) { return m_script; }

			BehaviourComponent * Clone(void) const;

		private:

			ScriptPointer m_script;

		};

	}

}