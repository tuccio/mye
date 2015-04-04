#ifndef __MYE_LIGHT__
#define __MYE_LIGHT__

struct Light
{
	float4 color;
	float4 position;
	float4 direction;

	float  spotAngle;
	float  range;
};

#endif