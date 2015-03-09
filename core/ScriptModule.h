#pragma once

#include "Module.h"
#include "Time.h"
#include "String.h"

#include "GameObjectsModule.h"

#include <vector>

namespace mye
{

	namespace core
	{

		enum ScriptMessageType
		{
			SCRIPT_MESSAGE_KEYBOARD_PRESSED,
			SCRIPT_MESSAGE_KEYBOARD_RELEASED,
			SCRIPT_MESSAGE_KEYBOARD_HELD,
			SCRIPT_MESSAGE_MOUSE_PRESSED,
			SCRIPT_MESSAGE_MOUSE_RELEASED,
			SCRIPT_MESSAGE_MOUSE_HELD,
			SCRIPT_MESSAGE_MOUSE_MOVED
		};

		struct ScriptMessage
		{
			
			ScriptMessageType   message;
			unsigned int        code;
			GameObjectHandle    hObj;
			
			union
			{

				void         * p;
				float          f;
				unsigned int   u;
				int            i;

				float          f2[2];
				float          f3[3];
				float          f4[4];

				int            i2[2];
				int            i3[3];
				int            i4[4];

				unsigned int   ui2[2];
				unsigned int   ui3[3];
				unsigned int   ui4[4];

			} data;

		};

		class ScriptModule :
			public Module
		{

		public:

			virtual bool Init(void) { return false; }
			virtual void Shutdown(void) { }

			virtual void Init(GameObjectHandle hObj) { }
			virtual void Finalize(GameObjectHandle hObj) { }

			virtual void Preupdate(FloatSeconds dt) { }
			virtual void Update(GameObjectsModule::Iterator it) { }

			void QueueMessage(const ScriptMessage & msg)
			{
				m_messages.push_back(msg);
			}

		protected:

			std::vector<ScriptMessage> m_messages;

		};

	}

}

/*

#include <string>

namespace mye
{

	namespace core
	{

		template <typename ConcreteFunctionCaller>
		class Script;

		template <typename ConcreteFunctionCaller>
		class ScriptModule :
			public IScriptModule
		{

		public:

			ScriptModule(void);
			~ScriptModule(void);

			Script<ConcreteFunctionCaller> LoadClass(const String &filename);
			Script<ConcreteFunctionCaller> LoadProcedure(const String &filename);

			bool Run(const String &filename);

		};

	}

}

#include "ScriptModule.inl"
*/
