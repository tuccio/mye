#include "common_samplers.hlsli"
#include "gamma.hlsli"
#include "quad_input.hlsli"

/* Source http://filmicgames.com/archives/75 */

Texture2D g_frame : register(t0);

#define MYE_TONEMAPPING_REINHARD_LWHITE 2.f
#define MYE_TONEMAPPING_REINHARD_LWHITE_SQUARED (MYE_TONEMAPPING_REINHARD_LWHITE * MYE_TONEMAPPING_REINHARD_LWHITE)


float3 Reinhard(in float3 color)
{

	// http://content.gpwiki.org/D3DBook:High-Dynamic_Range_Rendering
	
	return color * (1 + color / MYE_TONEMAPPING_REINHARD_LWHITE_SQUARED) / (1 + color);
	
}

float4 main(QuadInput input): SV_Target0
{

	float3 color = g_frame.Sample(g_pointSampler, input.texcoord).rgb;
	
	float4 output;
	
	output.rgb = Reinhard(color);
	output.a   = 1.f;
	
	return output;
	
}