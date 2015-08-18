#include "lpv.hlsli"
#include "register_slots.hlsli"

#include "spherical_harmonics.hlsli"

struct PSInput
{
	float4 positionCS : SV_Position;
	uint   depth      : SV_RenderTargetArrayIndex;
	float3 cell       : CELL;
};

struct PSOutput
{
	float4 shR : SV_Target0;
	float4 shG : SV_Target1;
	float4 shB : SV_Target2;
};

Texture3D<float4> g_lightVolumeRed   : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_RED);
Texture3D<float4> g_lightVolumeGreen : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_GREEN);
Texture3D<float4> g_lightVolumeBlue  : register(__MYE_DX11_TEXTURE_SLOT_LPVLIGHT_BLUE);
Texture3D<float4> g_geometryVolume   : register(__MYE_DX11_TEXTURE_SLOT_LPVGEOMETRY);

SamplerState      g_lpvSampler          : register(__MYE_DX11_SAMPLER_SLOT_LPV);


PSOutput main(PSInput input)
{

	LPV lpv = { g_lightVolumeRed, g_lightVolumeGreen, g_lightVolumeBlue, g_geometryVolume, g_lpvSampler };

	SHRGB sh;

	sh = LPVLoadOffset(lpv, input.cell, int3(0, 0, 0));

	LPVAddAdjacentContribution(sh, lpv, input.cell, float3(  0,  0, -1 ));
	LPVAddAdjacentContribution(sh, lpv, input.cell, float3(  0,  0,  1 ));
	LPVAddAdjacentContribution(sh, lpv, input.cell, float3(  0, -1,  0 ));
	LPVAddAdjacentContribution(sh, lpv, input.cell, float3(  0,  1,  0 ));
	LPVAddAdjacentContribution(sh, lpv, input.cell, float3( -1,  0,  0 ));
	LPVAddAdjacentContribution(sh, lpv, input.cell, float3(  1,  0,  0 ));

	PSOutput output;

	output.shR = sh.red;
	output.shG = sh.green;
	output.shB = sh.blue;

	return output;

}