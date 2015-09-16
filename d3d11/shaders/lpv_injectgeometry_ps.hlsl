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

	if (length(input.normal) < 0.01)
	{
		discard;
	}

	float4 shOccluder = SHCosineLobe(normalize(input.normal));

	//float4 shOccluder = SHCosineLobeLinear(float4(0, 0, 0, 2));

	float  areaRatio  = (input.surfelArea / (g_lpv.cellSize * g_lpv.cellSize));

	return SHScale(shOccluder, areaRatio);

}