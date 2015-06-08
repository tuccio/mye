#pragma once

namespace mye
{

	namespace core
	{

		class Cloneable
		{

		public:

			virtual Cloneable * Clone(void) const = 0;

		};

	}

}
