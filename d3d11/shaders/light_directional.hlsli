#ifndef __MYE_LIGHT_DIRECTIONAL__
#define __MYE_LIGHT_DIRECTIONAL__

#include "light.hlsli"
#include "constants.hlsli"

float3 LightVector(in Light light, in float3 x)
{
	return - light.direction.xyz;
}

float LightSpaceLinearDepth(in Light light, in float4 lightSpaceCoords, in float3 worldSpaceCoords)
{
	return lightSpaceCoords.z;
}

float3 LightIrradiance(in Light light, in float3 x)
{
	return light.color * light.intensity;
}

#endif