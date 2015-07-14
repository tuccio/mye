#pragma pack_matrix(row_major)

/* Input/Output structures */

struct VSInput
{
	float2 position  : POSITION;
	float2 texcoords : TEXCOORD0;
};

struct VSOutput
{
	float4 position  : SV_Position;
	float2 texcoords : TEXCOORD0;
};

cbuffer cbTransform : register(b0)
{
	float4x4 g_worldViewProj;
};

/* Main */

VSOutput main(VSInput input)
{

	VSOutput output;

	output.position  = mul(g_worldViewProj, float4(input.position, 0, 1));
	output.texcoords = input.texcoords;

	return output;

}