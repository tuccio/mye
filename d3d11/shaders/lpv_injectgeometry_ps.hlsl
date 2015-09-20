#include "spherical_harmonics.hlsli"
#include "constants.hlsli"
#include "lpv.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 normal     : NORMAL;
	float  surfelArea : SURFELAREA;
};

float4 main(PSInput input) : SV_Target0
{

	float len = length(input.normal);

	if (len < .01f)
	{
		discard;
	}

	float3 N          = input.normal / len;
	float4 shOccluder = SHCosineLobe(N);

	float  areaRatio = saturate(len * input.surfelArea / (g_lpv.cellSize * g_lpv.cellSize));

	return SHScale(shOccluder, areaRatio);

	return shOccluder;

}