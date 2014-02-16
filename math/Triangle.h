namespace mye
{

	namespace math
	{

		template <typename T>
		class Triangle
		{

		public:

			inline Matrix<T, 3, 1>& v0(void);
			inline const Matrix<T, 3, 1>& v0(void) const;

			inline Matrix<T, 3, 1>& v1(void);
			inline const Matrix<T, 3, 1>& v1(void) const;

			inline Matrix<T, 3, 1>& v2(void);
			inline const Matrix<T, 3, 1>& v2(void) const;

			inline bool operator== (const Triangle<T> &t);

		private:

			Matrix<T, 3, 1> m_v[3];

		};

		typedef Triangle<float> Trianglef;
		typedef Triangle<double> Triangled;

	}

}

#include "Triangle.inl"