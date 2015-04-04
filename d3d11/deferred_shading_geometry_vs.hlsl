#pragma pack_matrix(row_major)

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

};

struct VSOutput
{

	float4 positionCS  : SV_Position;
	float2 texcoord    : TEXCOORD;
	float3 normalWS    : NORMALWS;
	float3 positionWS  : POSITIONWS;

};

/* Main */

VSOutput main(VSInput input)
{

	VSOutput output;

	output.positionCS  = mul(g_worldViewProj, float4(input.position, 1));
	output.texcoord    = input.texcoord;
	output.normalWS    = mul((float3x3) g_world, input.normal);
	output.positionWS  = mul((float3x3) g_world, input.position);

	return output;

}