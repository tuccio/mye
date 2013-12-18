/* Ctors */

template <int N, typename T>
Vector<N, T>::Vector(void) : Vector(0)
{
}

template <int N, typename T>
Vector<N, T>::Vector(T *v)
{
	for (int i = 0; i < N; i++)
	{
		_v[i] = v[i];
	}
}

template <int N, typename T>
Vector<N, T>::Vector(T x)
{
	for (int i = 0; i < N; i++)
	{
		_v[i] = x;
	}
}

template <int N, typename T>
Vector<N, T>::Vector(T x, T y)
{
	_v[0] = x;
	_v[1] = y;
}

template <int N, typename T>
Vector<N, T>::Vector(T x, T y, T z)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
}

template <int N, typename T>
Vector<N, T>::Vector(T x, T y, T z, T w)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
	_v[3] = w;
}

/* Algebraic operations */

template <int N, typename T>
Vector<N, T> Vector<N, T>::operator+ (const Vector<N, T> &v) const
{

	Vector<N, T> r;

	for (int i = 0; i < N; i++)
	{
		r[i] = _v[i] + v[i];
	}

	return r;

}

template <int N, typename T>
Vector<N, T> Vector<N, T>::operator- (const Vector<N, T> &v) const
{

	Vector<N, T> r;

	for (int i = 0; i < N; i++)
	{
		r[i] = _v[i] - v[i];
	}

	return r;

}

template <int N, typename T>
Vector<N, T> Vector<N, T>::operator* (const Vector<N, T> &v) const
{

	Vector<N, T> r;

	for (int i = 0; i < N; i++)
	{
		r[i] = _v[i] * v[i];
	}

	return r;

}

template <int N, typename T>
Vector<N, T> Vector<N, T>::operator/ (const Vector<N, T> &v) const
{

	Vector<N, T> r;

	for (int i = 0; i < N; i++)
	{
		r[i] = _v[i] / v[i];
	}

	return r;

}

template <int N, typename T>
Vector<N, T>& Vector<N, T>::operator+= (const Vector<N, T> &v)
{

	for (int i = 0; i < N; i++)
	{
		_v[i] += v[i];
	}

	return *this;

}

template <int N, typename T>
Vector<N, T>& Vector<N, T>::operator-= (const Vector<N, T> &v)
{

	for (int i = 0; i < N; i++)
	{
		_v[i] -= v[i];
	}

	return *this;

}

template <int N, typename T>
Vector<N, T>& Vector<N, T>::operator*= (const Vector<N, T> &v)
{

	for (int i = 0; i < N; i++)
	{
		_v[i] *= v[i];
	}

	return *this;

}

template <int N, typename T>
Vector<N, T>& Vector<N, T>::operator/= (const Vector<N, T> &v)
{

	for (int i = 0; i < N; i++)
	{
		_v[i] /= v[i];
	}

	return *this;

}

template <int N, typename T>
T Vector<N, T>::Dot(const Vector<N, T> &v) const
{
	
	T d = 0;

	for (int i = 0; i < N; i++)
	{
		d += _v[i] * v._v[i];
	}

	return d;

}

template <int N, typename T>
Vector<N, T> Vector<N, T>::Cross(const Vector<N, T> &v) const
{
	throw 0;
}

template <int N, typename T>
float Vector<N, T>::Length(void) const
{

	T d = 0;

	for (int i = 0; i < N; i++)
	{
		d += _v[i] * _v[i];
	}

	return std::sqrt(d);

}

/* Members access */

template <int N, typename T>
const T& Vector<N, T>::operator[] (int i) const
{
	return _v[i];
};

template <int N, typename T>
T& Vector<N, T>::operator[] (int i)
{
	return _v[i];
};