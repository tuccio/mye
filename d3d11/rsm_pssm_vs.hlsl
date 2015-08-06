#pragma pack_matrix(row_major)

#include "pssm.hlsli"

/* Constant buffers */

cbuffer cbTransform : register(b0)
{
	float4x4 g_world;
	float4x4 g_worldViewMatrix;
	float4x4 g_worldViewProj;
};

cbuffer cbCrop : register(b1)
{
	float4x4 g_cropMatrix[__MYE_PSSM_MAX_SPLITS];
}


/* Input/Output structures */

struct VSInput
{

	float3 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal   : NORMAL;

	uint   instance : SV_InstanceID;

};

struct VSOutput
{

	float4 positionCS : SV_Position;

	float2 texcoord   : TEXCOORD;

	float3 normalWS   : NORMALWS;
	float3 positionWS : POSITIONWS;

	uint   slice      : SV_InstanceID;

};

/* Main */

VSOutput main(VSInput input)
{

	//float4x4 transform = mul(g_cropMatrix[input.instance], g_worldViewProj);
	float4x4 transform = mul(g_cropMatrix[input.instance], g_worldViewProj);

	VSOutput output;

	output.positionCS  = mul(transform, float4(input.position, 1));
	output.texcoord    = input.texcoord;
	output.normalWS    = mul((float3x3) g_world, input.normal);
	output.positionWS  = mul((float3x3) g_world, input.position);
	output.slice       = input.instance;

	return output;

}