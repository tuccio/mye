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

			inline float& a(void)
			{
				return m_data[3];
			}

			inline const float& a(void) const
			{
				return m_data[3];
			}

		private:

			float m_data[4];

		};

	}

}
