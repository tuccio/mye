#ifndef __MYE_GAUSS__
#define __MYE_GAUSS__

#include "constants.hlsli"

float GaussianWeight(float sigma, float distance)
{

	float s2 = sigma * sigma;
	float G  = 1.f / sqrt(2.f * MYE_PI * s2);

	return G * exp(-.5f * (distance * distance) / s2);

}

#endif