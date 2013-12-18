#pragma once

#include "Vector.h"

namespace mye
{

	namespace math
	{

		class Transform
		{

		public:

			Transform(void);
			~Transform(void);

			const Vector3f& GetPosition(void) const;
			void SetPosition(const Vector3f &p);
			Vector3f& Position(void);

		private:

			Vector3f _position;

		};

	}
}
