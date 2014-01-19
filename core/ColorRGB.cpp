#include "ColorRGB.h"
#include <cstring>

using namespace mye::core;

ColorRGB::ColorRGB(void)
{
}


ColorRGB::~ColorRGB(void)
{
}

ColorRGB::ColorRGB(float r,
				   float g,
				   float b)
{
	m_data[0] = r;
	m_data[1] = g;
	m_data[2] = b;
}

ColorRGB::ColorRGB(const float *data)
{
	memcpy(m_data, data, sizeof(float) * 3);
}