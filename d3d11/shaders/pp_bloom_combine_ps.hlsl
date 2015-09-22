#include "quad_input.hlsli"
#include "common_samplers.hlsli"
#include "luminance.hlsli"
#include "renderer_configuration.hlsli"

Texture2D <float4> g_frame : register(t0);
Texture2D <float4> g_bloom : register(t1);

float4 main(QuadInput input) : SV_Target0
{

	float3 color = g_frame.Sample(g_bilinearClampedSampler, input.texcoord).rgb;
	float3 bloom = g_bloom.Sample(g_bilinearClampedSampler, input.texcoord).rgb;
	
	return float4(color + r.ppBloomPower * bloom, 1);
	
}