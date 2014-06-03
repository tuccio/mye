#pragma pack_matrix(row_major)

cbuffer cbPerObject
{
	float4x4 g_worldViewProj;
};

float4 main(float4 position : POSITION) : SV_POSITION
{

	return mul(g_worldViewProj, position);

}