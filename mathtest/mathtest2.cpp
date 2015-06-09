#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <typeindex>
#include <type_traits>
#include <vector>

#include <mye/math/Math.h>
#include <mye/math/Geometry.h>

using namespace mye::math;

template <typename T, int N, int M>
void OctaveLikePrint(const char * matrixName, const Matrix<T, N, M> & m);

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, void> OctaveLikePrint(const char * label, T value);

template <typename T, int N, int M>
std::string MatrixToString(const Matrix<T, N, M> & m);

void PrintFrustum(const char * label, const Frustum & f);

int main(int argc, char * argv[])
{

	Vector3 t0 = Vector3(0);
	Vector3 t1 = Vector3(3, 2, 6);
	Vector3 N = t1.Normalize();
	Plane p(t0, N);
	Real k = -13.0f;

	Plane p1 = p.Transform(TranslationMatrix4(k * N)).Normalize();
	Plane p2 = p.TranslateAlongNormal(k).Normalize();
	Plane p3 = Plane(t0 + k * N, N).Normalize();

	OctaveLikePrint("p", p.Parameters());
	OctaveLikePrint("p transformed", p1.Parameters());
	OctaveLikePrint("p translated",  p2.Parameters());
	OctaveLikePrint("p constructed", p3.Parameters());

	system("pause");

	Frustum f(Vector3(0.0), Vector3(0, 0, 1), Vector3(0, 1, 0), Vector3(1, 0, 0), 0.1, 5.1, 30, 30);


	PrintFrustum("Constructed frustum", f);

	Frustum f1, f2;

	f.Split(0.3, f1, f2);

	PrintFrustum("Split frustum #1", f1);
	PrintFrustum("Split frustum #2", f2);

	system("pause");

	return 0;

}

void PrintFrustum(const char * label, const Frustum & f)
{

	std::cout << "*** " << label << " ***" << std::endl << std::endl;

	OctaveLikePrint("left", f.GetPlane(FrustumPlane::LEFT_PLANE).Parameters());
	OctaveLikePrint("right", f.GetPlane(FrustumPlane::RIGHT_PLANE).Parameters());
	OctaveLikePrint("near", f.GetPlane(FrustumPlane::NEAR_PLANE).Parameters());
	OctaveLikePrint("far", f.GetPlane(FrustumPlane::FAR_PLANE).Parameters());
	OctaveLikePrint("top", f.GetPlane(FrustumPlane::TOP_PLANE).Parameters());
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