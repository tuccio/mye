#pragma pack_matrix(row_major)

#include "camera_transform.hlsli"
#include "linear_depth.hlsli"

/* Constant buffers */

cbuffer cbTransform
{
	float4x4 g_world;
	float4x4 g_worldViewMatrix;
	float4x4 g_worldViewProj;
};

/* Input/Output structures */

struct VSInput
{
	float3 position  : POSITION;
	float2 texcoord  : TEXCOORD0;
	float3 normal    : NORMAL;
	float3 tangent   : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VSOutput
{

	float4 positionCS : SV_Position;
	float2 texcoord   : TEXCOORD;

};

/* Main */

VSOutput main(VSInput input)
{

	VSOutput output;

	float4 positionCS = mul(g_worldViewProj, float4(input.position, 1));
	output.positionCS = LinearizeDepth(positionCS);

	output.texcoord   = input.texcoord;

	return output;

}