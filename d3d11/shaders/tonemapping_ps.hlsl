#include "common_samplers.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	uint   instance   : SV_InstanceID;
};

Texture2D g_frame : register(t0);

float3 ReinhardTonemapping(in float3 color)
{
	return color / (1.f + color);
}

float4 main(PSInput input): SV_Target0
{

	float3 color = g_frame.Sample(g_pointSampler, input.texcoord).rgb;
	
	float4 output;
	
	output.rgb = ReinhardTonemapping(color);
	output.a   = 1.f;
	
	return output;
	
}