#ifndef __MYE_SPHERICAL_HARMONICS__
#define __MYE_SPHERICAL_HARMONICS__

#include "constants.hlsli"

#define MYE_COSLOBE_ZH_C0 (0.88622692545f)
#define MYE_COSLOBE_ZH_C1 (1.02332670795f)

struct SHRGB
{
	float4 red;
	float4 green;
	float4 blue;
};

SHRGB SHAdd(in SHRGB a, in SHRGB b)
{

	SHRGB r;

	r.red   = a.red   + b.red;
	r.green = a.green + b.green;
	r.blue  = a.blue  + b.blue;

	return r;

}

float SHDot(in float4 a, in float4 b)
{
	return dot(a, b);
}

float3 SHDot(in SHRGB a, in float4 b)
{
	return float3(dot(a.red, b), dot(a.green, b), dot(a.blue, b));
}

float SHDotAbs(in float4 a, in float4 b)
{
	return a.x * b.x + dot(abs(a.yzw), abs(b.yzw));
}

float3 SHDotAbs(in SHRGB a, in float4 b)
{
	return float3(SHDotAbs(a.red,   b),
	              SHDotAbs(a.green, b),
	              SHDotAbs(a.blue,  b));
}

float4 SHScale(in float4 a, in float b)
{
	return a * b;
}

SHRGB SHScale(in SHRGB a, in float3 b)
{

	SHRGB sh;

	sh.red   = a.red   * b.r;
	sh.green = a.green * b.g;
	sh.blue  = a.blue  * b.b;

	return sh;

}

SHRGB SHScale(in float4 a, in float3 b)
{
	SHRGB shRGB = { a, a, a };
	return SHScale(shRGB, b);
}

float4 SHCosineLobe(in float3 direction)
{
	return float4(MYE_COSLOBE_ZH_C0, - MYE_COSLOBE_ZH_C1 * direction.y, MYE_COSLOBE_ZH_C1 * direction.z, - MYE_COSLOBE_ZH_C1 * direction.x);
	//return float4(MYE_COSLOBE_ZH_C0, MYE_COSLOBE_ZH_C1 * direction.y, MYE_COSLOBE_ZH_C1 * direction.z, MYE_COSLOBE_ZH_C1 * direction.x);
	//return float4(MYE_COSLOBE_ZH_C0, - MYE_COSLOBE_ZH_C1, MYE_COSLOBE_ZH_C1, - MYE_COSLOBE_ZH_C1) * float4(1.f, direction.yzx);
}

#endif