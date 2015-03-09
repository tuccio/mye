#pragma once

#include <mye/core/Script.h>

#include <lua++11/lua++11.h>

namespace mye
{

	namespace lua
	{

		class BehaviourScript :
			public mye::core::Script
		{

		public:

			BehaviourScript(void);
			BehaviourScript(const mye::core::String &name);

			~BehaviourScript(void);

			inline luapp11::LuaBehaviour GetBehaviour(void) const
			{
				return m_behaviour;
			}

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

			luapp11::LuaBehaviour m_behaviour;

		};

		typedef std::shared_ptr<BehaviourScript> BehaviourScriptPointer;

	}

}