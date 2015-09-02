#ifndef __MYE_LPV__
#define __MYE_LPV__

#include "constants.hlsli"
#include "spherical_harmonics.hlsli"

struct LPVConfiguration
{

	float3 minCorner;
	float3 maxCorner;

	float  cellSize;

	uint   rsmResolution;
	uint   lpvResolution;

	float  geometryInjectionBias;
	float  fluxInjectionBias;

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

float3 LPVSampleCoords(in float3 cell)
{
	return cell / g_lpv.lpvResolution;
}

float4 LPVFlux(in SHRGB sh, in float3 direction)
{

	float4 shDirection = SHCosineLobe(direction);

	return float4(dot(shDirection, sh.red),
	              dot(shDirection, sh.green),
	              dot(shDirection, sh.blue),
	              1);

}

SHRGB LPVLoadOffset(in LPV lpv, int3 cell, int3 offset)
{

	SHRGB sh;

	sh.red   = lpv.red.Load(int4(cell + offset, 0));
	sh.green = lpv.green.Load(int4(cell + offset, 0));
	sh.blue  = lpv.blue.Load(int4(cell + offset, 0));

	return sh;

}

#ifndef MYE_PROPAGATE_NO_OCCLUSION

float4 LPVOcclusion(in LPV lpv, in int3 cell, in float3 sourceDirection)
{
	float3 coords = float3(cell) + sourceDirection;
	//float3 coords = float3(cell) + .5f + sourceDirection;
	//float3 coords = float3(cell) + 1.f + sourceDirection;
	float3 sampleCoords = LPVSampleCoords(coords);
	return lpv.geometry.Sample(lpv.lpvSampler, sampleCoords);
	//return lpv.geometry.Load(int4(coords, 0));
}

float LPVVisibility(in float4 shFluxDirection, in float4 shOcclusion)
{
	return 1 - saturate(SHDotAbs(shOcclusion, shFluxDirection));
}

#else

float4 LPVOcclusion(in LPV lpv, in int3 cell, in float3 sourceDirection)
{
	return float4(0, 0, 0, 0);
}

float LPVVisibility(in float4 shFluxDirection, in float4 shOcclusion)
{
	return 1;
}

#endif

#endif