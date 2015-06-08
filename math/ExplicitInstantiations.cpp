#include "Math.h"
#include "Geometry.h"

using namespace mye::math;

template class Matrix<float,        2, 1>;
template class Matrix<double,       2, 1>;
template class Matrix<int,          2, 1>;
template class Matrix<unsigned int, 2, 1>;

template class Matrix<float,        3, 1>;
template class Matrix<double,       3, 1>;
template class Matrix<int,          3, 1>;
template class Matrix<unsigned int, 3, 1>;

template class Matrix<float,        4, 1>;
template class Matrix<double,       4, 1>;
template class Matrix<int,          4, 1>;
template class Matrix<unsigned int, 4, 1>;

template class Matrix<double,       2, 2>;
template class Matrix<float,        2, 2>;
template class Matrix<int,          2, 2>;
template class Matrix<unsigned int, 2, 2>;

template class Matrix<double,       3, 3>;
template class Matrix<float,        3, 3>;
template class Matrix<int,          3, 3>;
template class Matrix<unsigned int, 3, 3>;
template class Matrix<Real,         3, 3>;

template class Matrix<double,       4, 4>;
template class Matrix<float,        4, 4>;
template class Matrix<int,          4, 4>;
template class Matrix<unsigned int, 4, 4>;
template class Matrix<Real,         4, 4>;