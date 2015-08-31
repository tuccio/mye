#pragma pack_matrix(row_major)

cbuffer cbTransform : register(b0)
{
	float4x4 g_world;
	float4x4 g_worldView;
	float4x4 g_worldViewProj;
};

struct VSInput
{
	float3 position : POSITION;
	float4 color    : DIFFUSE;
};

struct VSOutput
{
	float4 position : SV_Position;
	float4 color    : DIFFUSE;
};

VSOutput main(VSInput input)
{

	VSOutput output;

	output.position = mul(g_worldViewProj, float4(input.position, 1));
	output.color    = input.color;

	return output;

}