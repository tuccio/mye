#include <mye/math/Math.h>

#include <iostream>
#include <fstream>

using namespace mye::math;

#define RDTSC_DIRTY "%eax", "%ebx", "%ecx", "%edx"

#define RDTSC_START(cycles) { int tmp[4], x; __cpuid(tmp, x); cycles = __rdtsc(); }
#define RDTSC_STOP(cycles) { int tmp[4], x; __cpuid(tmp, x); cycles = __rdtsc(); __cpuid(tmp, x); }

int main(int argc, char * argv[])
{

construction:

	unsigned int t0, t1, t2, t3;

	RDTSC_START(t0);

	Matrix4 tmp0 = {
		1, 2, 3, 4,
		0, 1, 2, 3,
		0, 0, 1, 2,
		0, 0, 0, 1
	};

	RDTSC_STOP(t1);

inverse:

	RDTSC_START(t2);

	Matrix4 tmp1 = tmp0.Inverse();

	RDTSC_STOP(t3);

	std::ofstream f("out.tmp", std::ios::binary);

	f.write((const char *) tmp1.Data(), sizeof(tmp1));

	f.close();

	//std::cout << "Matrix4 construction cycles: " << t1 - t0 << std::endl;
	std::cout << "Matrix4 inverse cycles: " << t3 - t2 << std::endl;

	return 0;

}