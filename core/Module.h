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
			virtual void Shutdown(void);

			bool IsInitialized(void) const;

		protected:

			bool m_initialized;

		};

	}

}
