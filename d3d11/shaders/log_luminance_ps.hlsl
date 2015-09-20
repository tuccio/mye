#include "common_samplers.hlsli"
#include "quad_input.hlsli"
#include "luminance.hlsli"

#define MYE_LOG_LUMINANCE_DELTA .001f

Texture2D g_frame : register(t0);

float4 main(QuadInput input): SV_Target0
{

	float3 color        = g_frame.Sample(g_bilinearSampler, input.texcoord).rgb;
	float  logLuminance = log(MYE_LOG_LUMINANCE_DELTA + Luminance(color));
	
	return logLuminance;
	
}