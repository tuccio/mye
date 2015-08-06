#pragma pack_matrix(row_major)

#include "material.hlsli"
#include "light.hlsli"
#include "light_directional.hlsli"
#include "register_slots.hlsli"
#include "vsm_moments.hlsli"

/* Constant buffers */

cbuffer cbLight : register(__MYE_DX11_BUFFER_SLOT_LIGHT)
{
	Light g_light;
};

cbuffer cbMaterial : register(__MYE_DX11_BUFFER_SLOT_MATERIAL)
{
	Material g_material;
};

/* Input/Output structures */

struct PSInput
{

	float4 positionLS : SV_Position;
	float2 texcoord   : TEXCOORD;
	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

};

struct PSOutput
{
	float4 position : SV_Target0;
	float4 normal   : SV_Target1;
	float4 flux     : SV_Target2;
	float2 vsm      : SV_Target3;
};

/* Main */

PSOutput main(PSInput input)
{

	float3 N = normalize(input.normalWS);
	float3 L = LightVector(input.positionWS, g_light);

	float depth = LightSpaceLinearDepth(g_light, input.positionLS, input.positionWS);

	float4 NdotL = saturate(dot(N, L));

	PSOutput output;

	output.position = float4(input.positionWS, 1);
	output.normal   = float4(N, 1);
	output.flux     = g_light.intensity * g_light.color * g_material.diffuseColor * NdotL;
	output.vsm      = ComputeMoments(depth);

	return output;

}