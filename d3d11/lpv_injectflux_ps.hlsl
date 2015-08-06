#include "spherical_harmonics.hlsli"

struct PSInput
{

	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;

	float3 normal     : NORMAL;
	float3 flux       : FLUX;

};

struct PSOutput
{
	float4 shR : SV_Target0;
	float4 shG : SV_Target1;
	float4 shB : SV_Target2;
};

PSOutput main(PSInput input)
{

	PSOutput output;

	float4 sh = SHCosineLobe(input.normal);

	output.shR = sh * input.flux.r;
	output.shG = sh * input.flux.g;
	output.shB = sh * input.flux.b;

	return output;

}