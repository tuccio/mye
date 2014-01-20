#pragma once

namespace mye
{

	namespace core
	{

		class Resource;

		class ManualResourceLoader
		{

		public:

			ManualResourceLoader(void);
			~ManualResourceLoader(void);

			virtual bool Load(Resource *resource);
			virtual bool Prepare(Resource *resource);
			virtual void Unload(Resource *resource);

		protected:

			void CalculateSize(Resource *Resource);

		};

	}

}