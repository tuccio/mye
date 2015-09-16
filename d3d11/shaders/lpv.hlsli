#ifndef __MYE_LPV__
#define __MYE_LPV__

#include "common_samplers.hlsli"
#include "constants.hlsli"
#include "spherical_harmonics.hlsli"

struct LPVConfiguration
{

	float3 minCorner;
	float3 maxCorner;

	float  cellSize;

	uint   rsmSamples;
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

SHRGB LPVLoadOffset(in LPV lpv, in float3 cell, in int3 offset)
{

	SHRGB sh;

	sh.red   = lpv.red.Load(int4(cell + offset, 0));
	sh.green = lpv.green.Load(int4(cell + offset, 0));
	sh.blue  = lpv.blue.Load(int4(cell + offset, 0));

	/*sh.red   = lpv.red.Sample(g_pointSampler, cell + offset);
	sh.green = lpv.green.Sample(g_pointSampler, cell + offset);
	sh.blue  = lpv.blue.Sample(g_pointSampler, cell + offset);*/

	return sh;

}

#ifndef MYE_PROPAGATE_NO_OCCLUSION

float4 LPVOcclusion(in LPV lpv, in float3 cell, in float3 sourceDirection)
{
	float3 coords       = cell - .5f + sourceDirection * .5f;
	float3 sampleCoords = LPVSampleCoords(coords);
	return lpv.geometry.Sample(lpv.lpvSampler, sampleCoords);
	//return lpv.geometry.Load(int4(coords, 0));
}

float LPVVisibility(in float4 shOcclusion, in float fluxDirection)
{
	return 1.f - saturate(SHReconstruct(shOcclusion, fluxDirection));
}

#else

float4 LPVOcclusion(in LPV lpv, in float3 cell, in float3 sourceDirection)
{
	return float4(0, 0, 0, 0);
}

float LPVVisibility(in float4 shOcclusion, in float3 fluxDirection)
{
	return 1;
}

#endif

#endif