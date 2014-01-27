#pragma pack_matrix(row_major)

cbuffer cbPerObject
{
	float4x4 g_worldViewProj;
};

void main(float3 iPosition : POSITION,
		  out float4 oPosition : SV_POSITION)
{

	oPosition = mul(g_worldViewProj, float4(iPosition, 1));

}