#ifndef __MYE_LPV__
#define __MYE_LPV__

#include "constants.hlsli"
#include "spherical_harmonics.hlsli"

#define MYE_LPV_FRONT_SOLID_ANGLE (0.4006696846f)
#define MYE_LPV_SIDE_SOLID_ANGLE  (0.4234413544f)


struct LPVConfiguration
{

	float3 minCorner;
	float3 maxCorner;

	float  cellSize;

	uint   rsmResolution;
	uint   lpvResolution;

};

struct LPV
{
	Texture3D<float4> red;
	Texture3D<float4> green;
	Texture3D<float4> blue;
	Texture3D<float4> geometry;
	SamplerState      lpvSampler;
};

cbuffer cbLPV : register(b0)
{
	LPVConfiguration g_lpv;
};

float3 LPVGetGridCell(in float3 position)
{
	return (position - g_lpv.minCorner) / g_lpv.cellSize;
}

float3 LPVSampleCoords(in int3 cell)
{
	return float3(cell) / g_lpv.lpvResolution;
}

float4 LPVFlux(in SHRGB sh, in float3 direction)
{

	float4 shDirection = SHCosineLobe(direction);

	return float4(dot(shDirection, sh.red),
	              dot(shDirection, sh.green),
	              dot(shDirection, sh.blue),
	              1);

}

SHRGB LPVLoadOffset(in LPV lpv,
                    int3 cell,
                    int3 offset)
{

	SHRGB sh;

	sh.red   = lpv.red.Load(int4(cell + offset, 0));
	sh.green = lpv.green.Load(int4(cell + offset, 0));
	sh.blue  = lpv.blue.Load(int4(cell + offset, 0));

	return sh;

}

void LPVAddAdjacentContribution(inout SHRGB sh,
                                in LPV lpv,                                
                                in int3 cell,
                                in float3 sourceDirection)
{

	float3 fluxDirection = - sourceDirection;
	float4 shDirection   = SHCosineLobe(fluxDirection);

	SHRGB  shAdjRadiance = LPVLoadOffset(lpv, cell, sourceDirection);

	float3 shIrradiance  = saturate(SHDot(shAdjRadiance, shDirection));

	//float4 shOcclusion   = lpv.geometry.Sample(lpv.lpvSampler, LPVSampleCoords(cell + sourceDirection));
	//float  occlusion     = 1.f - SHDot(shOcclusion, shDirection);
	float  occlusion = 1.f;

	SHRGB  shFrontFlux   = SHScale(shDirection, MYE_LPV_FRONT_SOLID_ANGLE * MYE_INV_PI * occlusion * shIrradiance);

	sh = SHAdd(sh, shFrontFlux);


}

#endif