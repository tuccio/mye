#ifndef __MYE_LIGHT_DIRECTIONAL__
#define __MYE_LIGHT_DIRECTIONAL__

#include "light.hlsli"

void ComputeLightParams(in float3 x, in Light light, out float3 L, out float3 intensity)
{
	L         = - light.direction;
	intensity =   light.color.xyz;
}

float LightSpaceLinearDepth(in Light light, in float4 lightSpaceCoords, in float3 worldSpaceCoords)
{
	return lightSpaceCoords.z;
}

#endif