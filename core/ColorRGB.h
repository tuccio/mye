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

			inline const float* GetData(void) const
			{
				return m_data;
			}

			inline float& r(void)
			{
				return m_data[0];
			}

			inline const float& r(void) const
			{
				return m_data[0];
			}

			inline float& g(void)
			{
				return m_data[1];
			}

			inline const float& g(void) const
			{
				return m_data[1];
			}

			inline float& b(void)
			{
				return m_data[2];
			}

			inline const float& b(void) const
			{
				return m_data[2];
			}

		private:

			float m_data[3];

		};

	}

}
