#pragma once

namespace mye
{

	namespace core
	{

		class ColorRGB
		{

		public:

			ColorRGB(void);

			ColorRGB(float r,
				float g,
				float b);

			ColorRGB(const float *data);

			~ColorRGB(void);

			inline const float* Data(void) const;
			inline float* Data(void);

			inline const float& r(void) const;
			inline float& r(void);

			inline const float& g(void) const;
			inline float& g(void);

			inline const float& b(void) const;
			inline float& b(void);

		private:

			float m_data[3];

		};

	}

}

#include "ColorRGB.inl"