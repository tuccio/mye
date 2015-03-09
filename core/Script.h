#pragma once

#include "Resource.h"
#include "ResourceManager.h"

namespace mye
{

	namespace core
	{

		class Script :
			public mye::core::Resource
		{

		public:

			Script(void);
			Script(ResourceManager *rm, const String &name);

			~Script(void);

			void Clear(void);

		protected:

			size_t CalculateSizeImpl(void);

		};

		typedef std::shared_ptr<Script> ScriptPointer;

	}

}