#pragma once

namespace mye
{

	namespace core
	{

		class Module
		{

		public:

			Module(void);
			~Module(void);

			virtual bool Init(void);
			virtual void Clear(void);

		};

	}

}
