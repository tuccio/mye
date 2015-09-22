#include "common_samplers.hlsli"
#include "gamma.hlsli"
#include "quad_input.hlsli"
#include "renderer_configuration.hlsli"

/* Source http://filmicgames.com/archives/75 */

Texture2D g_frame : register(t0);

float3 Reinhard(in float3 color)
{
	float3 colorScaled = color * r.ppTonemapMiddleGrey / r.ppTonemapLogAverage;
	return colorScaled * (1 + colorScaled / (r.ppTonemapWhite * r.ppTonemapWhite)) / (1 + colorScaled);
}

float4 main(QuadInput input): SV_Target0
{

	float3 color = g_frame.Sample(g_pointSampler, input.texcoord).rgb;
	
	float4 output;
	
	output.rgb = Reinhard(color);
	output.a   = 1.f;
	
	return output;
	
}