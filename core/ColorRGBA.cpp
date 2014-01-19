#include "ColorRGBA.h"

#include <cstring>

using namespace mye::core;

ColorRGBA::ColorRGBA(void)
{
}

ColorRGBA::~ColorRGBA(void)
{
}

ColorRGBA::ColorRGBA(float r,
					 float g,
					 float b,
					 float a)
{
	m_data[0] = r;
	m_data[1] = g;
	m_data[2] = b;
	m_data[3] = a;
}

ColorRGBA::ColorRGBA(const float *data)
{
	memcpy(m_data, data, sizeof(float) * 4);
}


