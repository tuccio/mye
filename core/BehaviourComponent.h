#pragma once

#include "Component.h"
#include "Entity.h"
#include "Time.h"

#include <mye/lua/BehaviourScript.h>

namespace mye
{

	namespace core
	{

		class BehaviourComponent :
			public Component
		{

		public:

			BehaviourComponent(void);
			BehaviourComponent(const mye::lua::BehaviourScriptPointer &s);

			~BehaviourComponent(void);

			void Init(void);
			void Update(FloatSeconds dt);

			BehaviourComponent* Clone(void) const;

			inline mye::lua::BehaviourScriptPointer GetScript(void);

		private:

			mye::lua::BehaviourScriptPointer m_script;

		};

	}

}

#include "BehaviourComponent.inl"