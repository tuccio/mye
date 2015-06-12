#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <typeindex>
#include <type_traits>
#include <vector>

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

#include <boost/timer.hpp>

using namespace mye::math;

template <typename T, int N, int M>
void OctaveLikePrint(const char * matrixName, const Matrix<T, N, M> & m);

template <typename T>
void OctaveLikePrint(const char * matrixName, const QuaternionTempl<T> & q);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, void> OctaveLikePrint(const char * label, T value);

template <typename T, int N, int M>
std::string MatrixToString(const Matrix<T, N, M> & m);

void PrintFrustum(const char * label, const Frustum & f);

int main(int argc, char * argv[])
{



	Frustum f(Vector3(0),
	          Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0),
	          0.1f, 100.0f,
	          90.0f, 90.0f);
	

	for (auto corner : f.GetCorners())
	{
		OctaveLikePrint("corner", corner);
	}

	PrintFrustum("f", f);

	system("pause");

	Quaternion q1(Vector3(0, 1, 0), 45.0f);
	Quaternion q2(Vector3(0, 1, 0), 30.0f);

	OctaveLikePrint("q1", q1);
	OctaveLikePrint("q2", q2);

	OctaveLikePrint("q1 * q2", q1 * q2);

	system("pause");

	Quaternion q3(Vector3(0, 1, 0), 90.0f);
	Quaternion q4(Vector3(0, 1, 0), -90.0f);


	Vector3 r1 = q3.Rotate(Vector3(0, 0, 1));
	Vector3 r2 = q4.Rotate(Vector3(0, 0, 1));

	OctaveLikePrint("r1", r1);
	OctaveLikePrint("r2", r2);

	system("pause");

	return 0;

}

void PrintFrustum(const char * label, const Frustum & f)
{

	std::cout << "*** " << label << " ***" << std::endl << std::endl;

	OctaveLikePrint("left",   f.GetPlane(FrustumPlane::LEFT_PLANE).Parameters());
	OctaveLikePrint("right",  f.GetPlane(FrustumPlane::RIGHT_PLANE).Parameters());
	OctaveLikePrint("near",   f.GetPlane(FrustumPlane::NEAR_PLANE).Parameters());
	OctaveLikePrint("far",    f.GetPlane(FrustumPlane::FAR_PLANE).Parameters());
	OctaveLikePrint("top",    f.GetPlane(FrustumPlane::TOP_PLANE).Parameters());
	OctaveLikePrint("bottom", f.GetPlane(FrustumPlane::BOTTOM_PLANE).Parameters());

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

template <typename T>
void OctaveLikePrint(const char * matrixName, const QuaternionTempl<T> & q)
{
	OctaveLikePrint(matrixName, (const Vector4 &) q);
}