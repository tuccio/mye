#pragma pack_matrix(row_major)

#include "material.hlsli"
#include "register_slots.hlsli"

/* Constant buffers */

cbuffer cbMaterial : register(__MYE_DX11_BUFFER_SLOT_MATERIAL)
{
	Material g_material;
};

/* Input/Output structures */

struct PSInput
{

	float4 positionCS  : SV_Position;
	float3 normalWS    : NORMALWS;
	float3 positionWS  : POSITIONWS;

#ifdef MYE_USE_HEIGHT_MAP

	float2 texcoord    : TEXCOORD0;

	float3 eyeTS       : EYETS;

	float3 normalTS    : NORMALTS;

	float3 tangentWS   : TANGENTWS;
	float3 bitangentWS : BITANGENTWS;
#endif

};

struct PSOutput
{
	float4 gbuffer0 : SV_Target0;
	float4 gbuffer1 : SV_Target1;
};

#ifdef MYE_USE_HEIGHT_MAP

#include "parallax_mapping.hlsli"

#endif

/* Main */

PSOutput main(PSInput input) : SV_TARGET
{

	float3 x        = input.positionWS;
	float3 N        = input.normalWS;
	float  uv       = 0.f;
	
#ifdef MYE_USE_HEIGHT_MAP

	float2 texcoord = input.texcoord;

	float3x3 tangentToWorldSpace = {
		input.tangentWS,
		input.bitangentWS,
		input.normalWS
	};

	ParallaxMapping(input.eyeTS, input.normalTS, tangentToWorldSpace, texcoord, x, N);

	N = input.normalWS;

#endif

	PSOutput output;

	output.gbuffer0.xyz = N;
	output.gbuffer0.w   = g_material.specular;
	output.gbuffer1.xyz = x;
	output.gbuffer1.w   = uv;

	return output;

}