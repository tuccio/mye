/* Ctors */

template <typename T>
Vector<3, T>::Vector(T *v)
{
	_v[0] = v[0];
	_v[1] = v[1];
	_v[2] = v[2];
}

template <typename T>
Vector<3, T>::Vector(T x)
{
	_v[0] = x;
	_v[1] = x;
	_v[2] = x;
}

template <typename T>
Vector<3, T>::Vector(T x, T y)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = 0;
}

template <typename T>
Vector<3, T>::Vector(T x, T y, T z)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
}

/* Algebraic operations */

template <typename T>
Vector<3, T> Vector<3, T>::operator+ (const Vector<3, T> &v) const
{

	return Vector<3, T>(_v[0] + v[0],
		_v[1] + v[1],
		_v[2] + v[2]);

}

template <typename T>
Vector<3, T> Vector<3, T>::operator- (const Vector<3, T> &v) const
{

	return Vector<3, T>(_v[0] - v[0],
		_v[1] - v[1],
		_v[2] - v[2]);

}

template <typename T>
Vector<3, T> Vector<3, T>::operator* (const Vector<3, T> &v) const
{

	return Vector<3, T>(_v[0] * v[0],
		_v[1] * v[1],
		_v[2] * v[2]);

}

template <typename T>
Vector<3, T> Vector<3, T>::operator/ (const Vector<3, T> &v) const
{

	return Vector<3, T>(_v[0] / v[0],
		_v[1] / v[1],
		_v[2] / v[2]);

}

template <typename T>
Vector<3, T>& Vector<3, T>::operator+= (const Vector<3, T> &v)
{

	_v[0] += v[0];
	_v[1] += v[1];
	_v[2] += v[2];

	return *this;

}

template <typename T>
Vector<3, T>& Vector<3, T>::operator-= (const Vector<3, T> &v)
{

	_v[0] -= v[0];
	_v[1] -= v[1];
	_v[2] -= v[2];

	return *this;

}

template <typename T>
Vector<3, T>& Vector<3, T>::operator*= (const Vector<3, T> &v)
{

	_v[0] *= v[0];
	_v[1] *= v[1];
	_v[2] *= v[2];

	return *this;

}

template <typename T>
Vector<3, T>& Vector<3, T>::operator/= (const Vector<3, T> &v)
{

	_v[0] /= v[0];
	_v[1] /= v[1];
	_v[2] /= v[2];

	return *this;

}

template <typename T>
Vector<3, T>& Vector<3, T>::operator= (const Vector<3, T> &v)
{

	_v[0] = v[0];
	_v[1] = v[1];
	_v[2] = v[2];

	return *this;

}

template <typename T>
T Vector<3, T>::Dot(const Vector<3, T> &v) const
{
	return _v[0] * v._v[0] +
		_v[1] * v._v[1] +
		_v[2] * v._v[2];
}

template <typename T>
Vector<3, T> Vector<3, T>::Cross(const Vector<3, T> &v) const
{
	return Vector<3, T>(_v[1] * v._v[2] - _v[2] * v._v[1],
		_v[2] * v._v[0] - _v[0] * v._v[2],
		_v[0] * v._v[1] - _v[1] * v._v[0]);
}

template <typename T>
float Vector<3, T>::Length(void) const
{
	return std::sqrt(_v[0] * _v[0] +
		_v[1] * _v[1] +
		_v[2] * _v[2]);
}

/* Members access */

template <typename T>
const T& Vector<3, T>::operator[] (int i) const
{
	return _v[i];
};

template <typename T>
T& Vector<3, T>::operator[] (int i)
{
	return _v[i];
};

template <typename T>
T& Vector<3, T>::x(void)
{
	return _v[0];
}

template <typename T>
T& Vector<3, T>::y(void)
{
	return _v[1];
}

template <typename T>
T& Vector<3, T>::z(void)
{
	return _v[2];
}

template <typename T>
T Vector<3, T>::GetX(void) const
{
	return _v[0];
}

template <typename T>
void Vector<3, T>::SetX(float x)
{
	_v[0] = x;
}

template <typename T>
T Vector<3, T>::GetY(void) const
{
	return _v[1];
}

template <typename T>
void Vector<3, T>::SetY(float y)
{
	_v[1] = y;
}

template <typename T>
T Vector<3, T>::GetZ(void) const
{
	return _v[2];
}

template <typename T>
void Vector<3, T>::SetZ(float z)
{
	_v[2] = z;
}
