#pragma pack_matrix(row_major)

cbuffer cbPerObject
{
	float4x4 g_worldViewProj;
};

float4 main(float3 position : POSITION, float2 texcoords : TEXCOORD, float3 normal : NORMAL) : SV_Position
{

	return mul(g_worldViewProj, float4(position, 1));

}