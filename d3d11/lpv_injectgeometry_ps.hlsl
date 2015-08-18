#include "spherical_harmonics.hlsli"
#include "constants.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 normal     : NORMAL;
	float  surfelArea : SURFELAREA;
};

struct PSOutput
{
	float4 geometry : SV_Target0;
};

PSOutput main(PSInput input)
{

	if (length(input.normal) < 0.01)
	{
		discard;
	}

	float4 sh = SHCosineLobe(normalize(input.normal));

	PSOutput output;

	output.geometry = SHScale(sh, saturate(input.surfelArea));

	return output;

}