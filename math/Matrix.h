namespace mye
{
	namespace math
	{

		template <typename T, int ROWS, int COLS>
		class Matrix
		{

		public:

			Matrix(void);

			inline T& operator() (int i, int j);
			inline const T& operator() (int i, int j) const;

		protected:

			T m_data[ROWS * COLS];

		};

	}
}

#include "Matrix.inl"