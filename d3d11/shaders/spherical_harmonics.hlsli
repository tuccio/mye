#ifndef __MYE_SPHERICAL_HARMONICS__
#define __MYE_SPHERICAL_HARMONICS__

#include "constants.hlsli"

// Coefficients for the cosine lobe projection

#define MYE_COSLOBE_ZH_C0 (0.886226926f)
#define MYE_COSLOBE_ZH_C1 (1.023326708f)

// SH basis functions coefficients, used for the reconstruction

#define MYE_SH_BASIS_C0   (0.282094792f)
#define MYE_SH_BASIS_C1   (0.488602512f)

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
	return a.x * b.x + dot(abs(a.xyz), abs(b.xyz));
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

float4 SHCosineLobeLinear(in float4 hDirection)
{
	return float4(MYE_COSLOBE_ZH_C0, - MYE_COSLOBE_ZH_C1, MYE_COSLOBE_ZH_C1, - MYE_COSLOBE_ZH_C1) * hDirection.wyzx;
}

float4 SHCosineLobe(in float3 direction)
{
	return float4(MYE_COSLOBE_ZH_C0, - MYE_COSLOBE_ZH_C1 * direction.y, MYE_COSLOBE_ZH_C1 * direction.z, - MYE_COSLOBE_ZH_C1 * direction.x);
}

float SHReconstruct(in float4 sh, in float3 direction)
{
	float4 shEval = float4(MYE_SH_BASIS_C0, - MYE_SH_BASIS_C1 * direction.y, MYE_SH_BASIS_C1 * direction.z, - MYE_SH_BASIS_C1 * direction.x);
	return SHDot(sh, shEval);
}

float3 SHReconstruct(in SHRGB sh, in float3 direction)
{
	float4 shEval = float4(MYE_SH_BASIS_C0, - MYE_SH_BASIS_C1 * direction.y, MYE_SH_BASIS_C1 * direction.z, - MYE_SH_BASIS_C1 * direction.x);
	return SHDot(sh, shEval);
}

#endif