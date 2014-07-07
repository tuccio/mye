#pragma pack_matrix(row_major)

cbuffer cbTransform
{
	float4x4 g_world;
	float4x4 g_worldViewMatrix;
	float4x4 g_worldViewProj;
};

float4 main(float4 position : POSITION) : SV_POSITION
{

	return mul(g_worldViewProj, position);

}