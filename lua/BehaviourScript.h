#pragma once

#include "Script.h"

#include <luabind/luabind.hpp>

namespace mye
{

	namespace lua
	{

		class BehaviourScript :
			public Script
		{

		public:

			BehaviourScript(void);
			BehaviourScript(LuaModule &luaModule,
				const mye::core::String &name);

			~BehaviourScript(void);

			template <typename R>
			inline R Call(const mye::core::String &f) const;

			template <typename R, typename T1>
			inline R Call(const mye::core::String &f, T1 arg1) const;

			template <typename R, typename T1, typename T2>
			inline R Call(const mye::core::String &f, T1 arg1, T2 arg2) const;

		protected:

			bool LoadImpl(void);
			void UnloadImpl(void);

		};

		typedef boost::shared_ptr<BehaviourScript> BehaviourScriptPointer;

	}

}

#include "BehaviourScript.inl"