#include "quad_input.hlsli"
#include "common_samplers.hlsli"
#include "luminance.hlsli"
#include "renderer_configuration.hlsli"

Texture2D <float4> g_frame : register(t0);

float4 main(QuadInput input) : SV_Target0
{

	float3 color = g_frame.Sample(g_bilinearSampler, input.texcoord).rgb;
	
	if (Luminance(color) > r.ppBloomThreshold)
	{
		return float4(color, 1);
	}
	else
	{
		return float4(0, 0, 0, 0);
	}
	
}