#ifndef __MYE_GAMMA__
#define __MYE_GAMMA__

#include "constants.hlsli"
#include "renderer_configuration.hlsli"

float3 Gamma(float3 color)
{
	return pow(color, r.gamma);
}

#endif