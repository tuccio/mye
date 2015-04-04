//namespace mye
//{
//
//	namespace math
//	{
//
//
//		template <typename T>
//		Matrix<T, 4, 4> OrthographicProjection(const Matrix<T, 3, 1> & min, const Matrix<T, 3, 1> & max)
//		{
//
//			T iRmL = T(1) / (max.x() - min.x());
//			T iTmB = T(1) / (max.y() - min.y());
//			T iFmN = -T(1) / (max.z() - min.z());
//
//			Matrix<T, 4, 4> m(1);
//
//			m.m00() = -T(2) * iRmL;
//			m.m03() = (max.x() + min.x()) * iRmL;
//
//			m.m11() = T(2) * iTmB;
//			m.m13() = (max.y() + min.y()) * iTmB;
//
//			m.m22() = T(1) * iFmN;
//			m.m23() = -min.z() * iFmN;
//
//			return m;
//
//		}
//
//	}
//
//}