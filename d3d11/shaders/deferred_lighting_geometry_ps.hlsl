#pragma pack_matrix(row_major)

#include "material.hlsli"
#include "register_slots.hlsli"
#include "parallax_mapping.hlsli"
#include "common_samplers.hlsli"
#include "packing.hlsli"

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

#ifdef MYE_USE_NORMAL_HEIGHT_MAP

	float2 texcoord    : TEXCOORD0;

	float3 normalTS    : NORMALTS;

	float3 tangentWS   : TANGENTWS;
	float3 bitangentWS : BITANGENTWS;

#ifdef MYE_USE_PARALLAX
	float3 eyeTS       : EYETS;
#endif

#endif

};

struct PSOutput
{
	float4 gbuffer0 : SV_Target0;
	float4 gbuffer1 : SV_Target1;
};

#ifdef MYE_USE_NORMAL_HEIGHT_MAP

Texture2D g_normalHeightMap : register(__MYE_DX11_TEXTURE_SLOT_NORMALHEIGHTMAP);

#endif


/* Main */

PSOutput main(PSInput input) : SV_TARGET
{

	float3 x = input.positionWS;
	float3 N;
	
#ifdef MYE_USE_NORMAL_HEIGHT_MAP

	float2 texcoord = input.texcoord;

	float3x3 worldToTangentSpace = {
		input.tangentWS,
		input.bitangentWS,
		input.normalWS
	};

	float3x3 tangentToWorldSpace = transpose(worldToTangentSpace);

#ifdef MYE_USE_PARALLAX

	//ParallaxMapping(g_normalHeightMap, input.eyeTS, input.normalTS, tangentToWorldSpace, texcoord, x, N);

#else

	// Plain normal mapping

	float3 sampledNormalTS = g_normalHeightMap.Sample(g_anisotropicSampler, input.texcoord).rgb;
	
	N = normalize(mul(tangentToWorldSpace, 2.f * sampledNormalTS - 1.f));

#endif

#else

	N = normalize(input.normalWS);

#endif

	PSOutput output;

	output.gbuffer0.xyz = N;
	output.gbuffer0.w   = g_material.specular;

	output.gbuffer1.xyz = x;
	output.gbuffer1.w   = 1.f;

	return output;

}