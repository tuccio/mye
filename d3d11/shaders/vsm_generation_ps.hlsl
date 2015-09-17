#include "vsm_moments.hlsli"
#include "common_samplers.hlsli"
#include "register_slots.hlsli"
#include "esm.hlsli"
#include "quad_input.hlsli"

Texture2D<float> g_depth : register(__MYE_DX11_TEXTURE_SLOT_SHADOWMAP);

#ifdef MYE_SHADOW_MAP_EVSM

float4 main(QuadInput input) : SV_Target0
{

	float  depth  = g_depth.SampleLevel(g_bilinearSampler, input.texcoord, 0);
	float2 wDepth = ESMWarp(depth);
	
	return float4(wDepth, wDepth * wDepth);
	
}

#else

float2 main(QuadInput input) : SV_Target0
{
	float depth = g_depth.SampleLevel(g_bilinearSampler, input.texcoord, 0);
	return VSMComputeMoments(depth);
}

#endif
