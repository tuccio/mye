#pragma pack_matrix(row_major)

#include "material.hlsli"

/* Constant buffers */

cbuffer cbMaterial
{
	Material g_material;
};

/* Input/Output structures */

struct PSInput
{

	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;
	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

};

struct PSOutput
{
	float4 gbuffer0 : SV_Target0;
	float4 gbuffer1 : SV_Target1;
};

/* Main */

PSOutput main(PSInput input) : SV_TARGET
{

	PSOutput output;

	output.gbuffer0 = float4(normalize(input.normalWS), g_material.specular);
	output.gbuffer1 = float4(input.positionWS, 1);

	return output;

}