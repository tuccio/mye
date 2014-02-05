#pragma once

namespace mye
{

	namespace core
	{

		class ColorRGBA
		{

		public:

			ColorRGBA(void);

			ColorRGBA(float r,
				float g,
				float b,
				float a);

			ColorRGBA(const float *data);

			~ColorRGBA(void);

			inline const float* Data(void) const;
			inline float* Data(void);

			inline const float& r(void) const;
			inline float& r(void);

			inline const float& g(void) const;
			inline float& g(void);

			inline const float& b(void) const;
			inline float& b(void);

			inline const float& a(void) const;
			inline float& a(void);

			inline ColorRGBA operator+ (const ColorRGBA &color) const;

			inline static ColorRGBA White(void);
			inline static ColorRGBA Black(void);
			inline static ColorRGBA Red(void);
			inline static ColorRGBA Green(void);
			inline static ColorRGBA Blue(void);

		private:

			float m_data[4];

		};

	}

}

#include "ColorRGBA.inl"