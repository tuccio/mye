#include <mye/math/Vector.h>
#include <mye/math/ToString.h>

#include <iostream>

using namespace mye::math;
using namespace std;

int main(int argc, char *argv[])
{

	Vector3i v0(0, 1, 0);
	Vector3i v1(1);

	cout << mye::math::ToString(v0 + v1) << endl;
	cout << mye::math::ToString(v0 - v1) << endl;
	cout << mye::math::ToString(v0 * v1) << endl;
	cout << mye::math::ToString(v0 / v1) << endl;

	Vector3i v2;

	v2.x() = 1;
	v2.y() = 2;
	v2.z() = 3;

	v2 -= v1;

	cout << mye::math::ToString(v2) << endl;

	system("pause");

	return 0;

}