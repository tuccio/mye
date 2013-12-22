#include "Variable.h"

#define __IFTYPE_CAST(_TYPE, _CASTTYPE) (_type == _TYPE ? (_CASTTYPE*) _data : NULL)

using namespace mye::core;

Variable::Variable(void) :
	_type(MYE_NULL),
	_data(NULL)
{
}

Variable::~Variable(void)
{
	Clear();
}

void Variable::SetNull(void)
{

	Clear();

	_data = NULL;
	_type = MYE_NULL;

}

Variable::Type Variable::GetType(void)
{
	return _type;
}

void Variable::SetFloat(float x)
{

	Clear();

	_data = new float;
	_type = MYE_FLOAT;

}

float Variable::GetFloat(void) const
{

	float *x = __IFTYPE_CAST(MYE_FLOAT, float);

	if (x)
	{
		return *x;
	}

	throw;

}

void Variable::SetVec3(const Eigen::Vector3f &v)
{

	Clear();

	_data = new Eigen::Vector3f;
	_type = MYE_VEC3;

}

Eigen::Vector3f Variable::GetVec3(void) const
{

	Eigen::Vector3f *x = __IFTYPE_CAST(MYE_VEC3, Eigen::Vector3f);

	if (x)
	{
		return *x;
	}

	throw;

}

void Variable::SetPointer(void *p)
{

	Clear();

	_data = p;
	_type = MYE_POINTER;

}

void* Variable::GetPointer(void) const
{

	if (_type == MYE_POINTER)
	{
		return _data;
	}
	
	throw;

}

void Variable::Clear(void)
{

	if (_data && _type != MYE_POINTER)
	{
		delete _data;
	}

}