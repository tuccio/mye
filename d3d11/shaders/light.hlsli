#ifndef __MYE_LIGHT__
#define __MYE_LIGHT__

struct Light
{

	float3 color;
	float3 position;
	float3 direction;

	float  intensity;
	float  spotAngle;
	float  range;

};

#endif