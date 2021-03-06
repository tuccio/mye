#pragma once

#include <functional>
#include "ManualResourceLoader.h"

namespace mye
{

	namespace core
	{

		class ManualLambdaLoader :
			public ManualResourceLoader
		{

		public:

			ManualLambdaLoader(
				std::function<bool(Resource*)> load,
				std::function<void(Resource*)> unload);

			bool Load(Resource *resource);
			void Unload(Resource *resource);

			~ManualLambdaLoader(void);

		private:

			std::function<bool(Resource*)> m_load;
			std::function<void(Resource*)> m_unload;


		};

	}

}
