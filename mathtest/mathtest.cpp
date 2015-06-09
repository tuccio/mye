#include <mye/math/Math.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <typeindex>
#include <type_traits>
#include <vector>

#include <boost/timer.hpp>

using namespace mye::math;

#define TEST_PRINT_ALIGNMENT 40

template <typename T, int N, int M>
bool MatrixNaiveEqual(const Matrix<T, N, M> & A, const Matrix<T, N, M> & B);

template <typename T, int N, int M>
Matrix<T, N, M> MatrixNaiveSum(const Matrix<T, N, M> & A, const Matrix<T, N, M> & B);

template <typename T, int N, int M>
Matrix<T, N, M> MatrixNaiveDifference(const Matrix<T, N, M> & A, const Matrix<T, N, M> & B);

template <typename T, int N, int M, int K>
Matrix<T, N, K> MatrixNaiveMultiplication(const Matrix<T, N, M> & A, const Matrix<T, M, K> & B);

template <typename T, int N, int M>
std::string MatrixToString(const Matrix<T, N, M> & m);

template <typename T, int N, int M>
void OctaveLikePrint(const char * matrixName, const Matrix<T, N, M> & m);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, void> OctaveLikePrint(const char * label, T value);

template <typename T, int N, int M>
T MatrixIncrementalValue(int i, int j);

template <typename T, int N, int M>
T MatrixAlternateIncrementalValue(int i, int j);

template <typename T, int N, int M>
T MatrixCircularVectorValue(int i, int j);

template <typename T, int N, int M>
void MatrixFill(Matrix<T, N, M> & m, T (*Setter) (int, int));

bool MatrixSumTest(void);
bool MatrixDifferenceTest(void);
bool MatrixMultiplicationTest(void);

int main(int argc, char * argv[])
{

	MatrixSumTest();
	MatrixDifferenceTest();
	MatrixMultiplicationTest();

	system("pause");


	Vector4f v1(1, -2, 3, -4);
	Vector4f v2(-3, 4, -5, 6);

	OctaveLikePrint("v1", v1);
	OctaveLikePrint("v2", v2);

	OctaveLikePrint("-v1", -v1);
	OctaveLikePrint("-v2", -v2);

	OctaveLikePrint("|v1|", v1.CwiseAbs());
	OctaveLikePrint("|v2|", v2.CwiseAbs());

	OctaveLikePrint("dot(v1, v2)", v1.Dot(v2));

	OctaveLikePrint("normalize(v1, v2)", v1.Normalize());
	//OctaveLikePrint("normalize_fast(v1, v2)", v1.NormalizeFast());

	OctaveLikePrint("length(normalize(v1, v2))", v1.Normalize().Length());
	//OctaveLikePrint("length(normalize_fast(v1, v2))", v1.NormalizeFast().Length());

	OctaveLikePrint("length(v1)", v1.Length());
	OctaveLikePrint("length(v2)", v2.Length());

	OctaveLikePrint("clamp(v1, 0, 1)", v1.Clamp(0, 1));
	OctaveLikePrint("clamp(v2, 0, 1)", v2.Clamp(0, 1));

	OctaveLikePrint("clamp(v1, -1, 1)", v1.Clamp(-1, 1));
	OctaveLikePrint("clamp(v2, -1, 1)", v2.Clamp(-1, 1));

	OctaveLikePrint("max(v1, v2)", v1.CwiseMax(v2));
	OctaveLikePrint("min(v1, v2)", v1.CwiseMin(v2));

	Vector3f a(2, -1, 4);
	Vector3f b(4, 6, 12);

	OctaveLikePrint("a", a);
	OctaveLikePrint("b", b);

	OctaveLikePrint("cross(a, b)", a.Cross(b));

	OctaveLikePrint("a + b", a + b);
	OctaveLikePrint("a - b", a - b);

	OctaveLikePrint("-a", -a);
	OctaveLikePrint("-b", -b);

	OctaveLikePrint("normalize(a)", a.Normalize());
	OctaveLikePrint("length(normalize(a))", a.Normalize().Length());

	Matrix4f m1, m2;

	MatrixFill(m1, MatrixIncrementalValue<float, 4, 4>);
	MatrixFill(m2, MatrixCircularVectorValue<float, 4, 4>);

	OctaveLikePrint("m1", m1);
	OctaveLikePrint("m2", m2);

	OctaveLikePrint("determinant(m1)", m1.Determinant());
	OctaveLikePrint("determinant(m2)", m2.Determinant());

	OctaveLikePrint("inverse(m2)", m2.Inverse());
	OctaveLikePrint("m2 * inverse(m2)", m2 * m2.Inverse());

	Matrix3f m3;

	MatrixFill(m3, MatrixIncrementalValue<float, 3, 3>);

	OctaveLikePrint("m3", m3);
	OctaveLikePrint("Matrix4(m3)", Matrix4f(m3));

	Matrix3f m4;
	MatrixFill(m4, MatrixCircularVectorValue<float, 3, 3>);

	OctaveLikePrint("m4", m4);
	OctaveLikePrint("tranpose(m3)", m3.Transpose());
	OctaveLikePrint("tranpose(m4)", m4.Transpose());

	OctaveLikePrint("determinant(m3)", m3.Determinant());
	OctaveLikePrint("determinant(m4)", m4.Determinant());

	OctaveLikePrint("inverse(m4)", m4.Inverse());
	OctaveLikePrint("m4 * inverse(m4)", m4 * m4.Inverse());

	Matrix2f m5, m6;

	MatrixFill(m5, MatrixIncrementalValue<float, 2, 2>);
	MatrixFill(m6, MatrixCircularVectorValue<float, 2, 2>);

	OctaveLikePrint("m5", m5);
	OctaveLikePrint("m6", m6);

	OctaveLikePrint("tranpose(m5)", m5.Transpose());
	OctaveLikePrint("tranpose(m6)", m6.Transpose());

	OctaveLikePrint("determinant(m5)", m5.Determinant());
	OctaveLikePrint("determinant(m6)", m6.Determinant());

	OctaveLikePrint("inverse(m6)", m5.Inverse());
	OctaveLikePrint("inverse(m6)", m6.Inverse());

	OctaveLikePrint("m5 * inverse(m5)", m5 * m5.Inverse());
	OctaveLikePrint("m6 * inverse(m6)", m6 * m6.Inverse());

	Vector2f v3(0, 1);
	Vector2f v4(2, 3);

	OctaveLikePrint("v3", v3);
	OctaveLikePrint("v4", v4);

	OctaveLikePrint("(v3, 2)", Vector3f(v3, 2));
	OctaveLikePrint("(v4, 4, 5)", Vector4f(v4, 4, 5));

	system("pause");

	OctaveLikePrint("v2.xyz", v2.xyz());
	OctaveLikePrint("v2.yyw", v2.yyw());
	OctaveLikePrint("v3.xyyx", v3.xyyx());

	OctaveLikePrint("Sqrt(2)",    Sqrt(2.0f));
	OctaveLikePrint("InverseSqrt(2)",    InverseSqrt(2.0f));
	OctaveLikePrint("Sqrt(2, 3, 4, 5)", Sqrt(Vector4(2, 3, 4, 5)));
	OctaveLikePrint("Sqrt(6, 7, 8, 9)", InverseSqrt(Vector4(6, 7, 8, 9)));

	system("pause");

	return 0;

}

template <typename T, int N, int M>
bool MatrixSumTestTemplate(void)
{

	std::stringstream ss;

	ss << "MatrixSumTest<" << std::type_index(typeid(T)).name() << ", " << N << ", " << M << ">";

	Matrix<T, N, M> A, B, C;

	typedef T (*Setter) (int, int);

	Setter setters[] = { MatrixIncrementalValue<T, N, M>, MatrixAlternateIncrementalValue<T, N, M> };

	for (Setter setter1 : setters)
	{

		for (Setter setter2 : setters)
		{
			MatrixFill(A, setter1);
			MatrixFill(B, setter2);

			C = A + B;
			auto R = MatrixNaiveSum(A, B);

			if (!MatrixNaiveEqual(C, R))
			{

				std::cout << std::left << std::setw(TEST_PRINT_ALIGNMENT) << ss.str() << " : FAILED" << std::endl;

				OctaveLikePrint("A", A);
				OctaveLikePrint("B", B);
				OctaveLikePrint("A + B", C);
				OctaveLikePrint("Correct", R);

				return false;

			}

		}

	}

	std::cout << std::left << std::setw(TEST_PRINT_ALIGNMENT) << ss.str() << " : OK" << std::endl;

	return true;

}

template <typename T, int N, int M>
bool MatrixDifferenceTestTemplate(void)
{

	std::stringstream ss;

	ss << "MatrixDifferenceTest<" << std::type_index(typeid(T)).name() << ", " << N << ", " << M << ">";

	Matrix<T, N, M> A, B, C;

	typedef T(*Setter) (int, int);

	Setter setters[] = { MatrixIncrementalValue<T, N, M>, MatrixAlternateIncrementalValue<T, N, M> };

	for (Setter setter1 : setters)
	{

		for (Setter setter2 : setters)
		{
			MatrixFill(A, setter1);
			MatrixFill(B, setter2);

			C = A - B;
			auto R = MatrixNaiveDifference(A, B);

			if (!MatrixNaiveEqual(C, R))
			{

				std::cout << std::left << std::setw(TEST_PRINT_ALIGNMENT) << ss.str() << " : FAILED" << std::endl;

				OctaveLikePrint("A", A);
				OctaveLikePrint("B", B);
				OctaveLikePrint("A - B", C);
				OctaveLikePrint("Correct", R);

				return false;

			}

		}

	}

	std::cout << std::left << std::setw(TEST_PRINT_ALIGNMENT) << ss.str() << " : OK" << std::endl;

	return true;

}

template <typename T, int N, int M, int K>
bool MatrixMultiplicationTestTemplate(void)
{

	std::stringstream ss;

	ss << "MatrixMultiplicationTest<" << std::type_index(typeid(T)).name() << ", " << N << ", " << M << ", " << K << ">";

	Matrix<T, N, M> A;
	Matrix<T, M, K> B;
	Matrix<T, N, K> C;

	typedef T(*Setter) (int, int);

	Setter setters[] = { MatrixIncrementalValue<T, N, M>, MatrixAlternateIncrementalValue<T, N, M> };

	for (Setter setter1 : setters)
	{

		for (Setter setter2 : setters)
		{
			MatrixFill(A, setter1);
			MatrixFill(B, setter2);

			C = A * B;
			auto R = MatrixNaiveMultiplication(A, B);

			if (!MatrixNaiveEqual(C, R))
			{

				std::cout << std::left << std::setw(TEST_PRINT_ALIGNMENT) << ss.str() << " : FAILED" << std::endl;

				OctaveLikePrint("A", A);
				OctaveLikePrint("B", B);
				OctaveLikePrint("A * B", C);
				OctaveLikePrint("Correct", R);

				return false;

			}

		}

	}

	std::cout << std::left << std::setw(TEST_PRINT_ALIGNMENT) << ss.str() << " : OK" << std::endl;

	return true;

}

bool MatrixSumTest(void)
{

	return MatrixSumTestTemplate<Real, 2, 2>() &&
	       MatrixSumTestTemplate<Real, 3, 3>() &&
	       MatrixSumTestTemplate<Real, 4, 4>() &&
	       MatrixSumTestTemplate<Real, 2, 1>() &&
	       MatrixSumTestTemplate<Real, 3, 1>() &&
	       MatrixSumTestTemplate<Real, 4, 1>() &&
		   MatrixSumTestTemplate<int,  2, 2>() &&
	       MatrixSumTestTemplate<int,  3, 3>() &&
	       MatrixSumTestTemplate<int,  4, 4>() &&
	       MatrixSumTestTemplate<int,  2, 1>() &&
	       MatrixSumTestTemplate<int,  3, 1>();

}

bool MatrixDifferenceTest(void)
{

	return MatrixDifferenceTestTemplate<Real, 2, 2>() &&
	       MatrixDifferenceTestTemplate<Real, 3, 3>() &&
	       MatrixDifferenceTestTemplate<Real, 4, 4>() &&
	       MatrixDifferenceTestTemplate<Real, 2, 1>() &&
	       MatrixDifferenceTestTemplate<Real, 3, 1>() &&
	       MatrixDifferenceTestTemplate<Real, 4, 1>() &&
		   MatrixDifferenceTestTemplate<int,  2, 2>() &&
	       MatrixDifferenceTestTemplate<int,  3, 3>() &&
	       MatrixDifferenceTestTemplate<int,  4, 4>() &&
	       MatrixDifferenceTestTemplate<int,  2, 1>() &&
	       MatrixDifferenceTestTemplate<int,  3, 1>();

}

bool MatrixMultiplicationTest(void)
{

	return MatrixMultiplicationTestTemplate<Real, 3, 3, 3>() &&
	       MatrixMultiplicationTestTemplate<Real, 4, 4, 4>() &&
	       MatrixMultiplicationTestTemplate<int,  3, 3, 3>() &&
	       MatrixMultiplicationTestTemplate<int,  4, 4, 4>() &&
	       MatrixMultiplicationTestTemplate<Real, 3, 3, 1>() &&
	       MatrixMultiplicationTestTemplate<Real, 4, 4, 1>();

}

template <typename T, int N, int M>
bool MatrixNaiveEqual(const Matrix<T, N, M> & A, const Matrix<T, N, M> & B)
{

	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < M; j++)
		{

			if (A(i, j) != B(i, j))
			{
				return false;
			}

		}

	}

	return true;

}

template <typename T, int N, int M>
Matrix<T, N, M> MatrixNaiveSum(const Matrix<T, N, M> & A, const Matrix<T, N, M> & B)
{

	Matrix<T, N, M> C;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < M; j++)
		{

			C(i, j) = A(i, j) + B(i, j);

		}

	}

	return C;

}

template <typename T, int N, int M>
Matrix<T, N, M> MatrixNaiveDifference(const Matrix<T, N, M> & A, const Matrix<T, N, M> & B)
{

	Matrix<T, N, M> C;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < M; j++)
		{

			C(i, j) = A(i, j) - B(i, j);

		}

	}

	return C;

}


template <typename T, int N, int M, int K>
Matrix<T, N, K> MatrixNaiveMultiplication(const Matrix<T, N, M> & A, const Matrix<T, M, K> & B)
{

	Matrix<T, N, K> C;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < K; j++)
		{

			T v = 0;

			for (int k = 0; k < M; k++)
			{

				v += A(i, k) * B(k, j);

			}

			C(i, j) = v;

		}

	}

	return C;

}

template <typename T, int N, int M>
std::string MatrixToString(const Matrix<T, N, M> & m)
{

	std::stringstream ss;

	std::vector<std::string> values;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < M; j++)
		{
			values.push_back(std::to_string(m(i, j)));
		}

	}

	int align = std::max_element(values.begin(), values.end(), [] (const std::string & a, const std::string & b) { return a.size() < b.size(); })->size() + 1;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0; j < M; j++)
		{

			ss << std::setw(align) << values[i * M + j];

		}

		ss << std::endl;

	}


	return ss.str();

}

template <typename T, int N, int M>
void OctaveLikePrint(const char * matrixName, const Matrix<T, N, M> & m)
{
	std::cout << matrixName << " = " << std::endl << std::endl << MatrixToString(m) << std::endl;
}

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, void> OctaveLikePrint(const char * label, T value)
{
	std::cout << label << " = " << std::endl << std::endl << " " << value << std::endl << std::endl;
}

template <typename T, int N, int M>
T MatrixIncrementalValue(int i, int j)
{
	return T(i * N + j + 1);
}

template <typename T, int N, int M>
T MatrixAlternateIncrementalValue(int i, int j)
{
	return std::pow(-1, i + j) * MatrixIncrementalValue<T, N, M>(i, j);
}

template <typename T, int N, int M>
T MatrixCircularVectorValue(int i, int j)
{

	int K = std::max(N, M);

	int m = (K + i + j) % K;

	return m;

}

template <typename T, int N, int M>
void MatrixFill(Matrix<T, N, M> & m, T(*Setter) (int, int))
{

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			m(i, j) = Setter(i, j);
		}
	}

}

