#ifndef __MYE_GAMMA__
#define __MYE_GAMMA__

#include "constants.hlsli"
#include "renderer_configuration.hlsli"

float3 Gamma(float3 color)
{
	return pow(color, r.gamma);
}

float3 InverseGamma(float3 color)
{
	return pow(color, 1.f / r.gamma);
}

#endif