#include "spherical_harmonics.hlsli"
#include "constants.hlsli"
#include "lpv.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 normal     : NORMAL;
	float3 flux       : FLUX;
};

struct PSOutput
{
	float4 red      : SV_Target0;
	float4 green    : SV_Target1;
	float4 blue     : SV_Target2;
};

PSOutput main(PSInput input)
{

	if (length(input.normal) < 0.01)
	{
		discard;
	}

	float3 N  = normalize(input.normal);
	float4 sh = SHCosineLobe(N);
	
	float3 intensity = input.flux;

	PSOutput output;

	output.red      = SHScale(sh, intensity.r);
	output.green    = SHScale(sh, intensity.g);
	output.blue     = SHScale(sh, intensity.b);

	return output;

}