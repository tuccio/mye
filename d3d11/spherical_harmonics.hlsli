#ifndef __MYE_SPHERICAL_HARMONICS__
#define __MYE_SPHERICAL_HARMONICS__

float4 SHCosineLobe(float3 direction)
{
	return float4(.25f, - .5f * direction.y, .5f * direction.z, - .5f * direction.x);
}

#endif