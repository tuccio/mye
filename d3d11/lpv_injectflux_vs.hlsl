#include "register_slots.hlsli"

Texture2D<float4> g_position : register(__MYE_DX11_TEXTURE_SLOT_RSMPOSITION);
Texture2D<float4> g_normal   : register(__MYE_DX11_TEXTURE_SLOT_RSMNORMAL);
Texture2D<float4> g_flux     : register(__MYE_DX11_TEXTURE_SLOT_RSMALBEDO);

struct VSInput
{
	uint vertexID : SV_VertexID;
};

struct VSOutput
{
	float4 cell   : SV_Position;
	float3 normal : NORMAL;
	float3 flux   : FLUX;
};

cbuffer cbInject
{

	struct LPVInfo
	{

		float3 minCorner;
		float3 maxCorner;

		float  cellSize;

		uint   rsmResolution;
		uint   lpvResolution;

	} g_lpv;
	

};

VSOutput main(VSInput input)
{

	uint i = input.vertexID / g_lpv.rsmResolution;
	uint j = input.vertexID % g_lpv.rsmResolution;

	int3 texcoords = int3(i, j, 0);

	float4 position = g_position.Load(texcoords);

	VSOutput output;

	output.normal = g_normal.Load(texcoords).xyz;
	output.flux   = g_flux.Load(texcoords).xyz;
	output.cell   = float4(int3((position - g_lpv.minCorner) / g_lpv.cellSize + .5f * output.normal), 1.f);

	return output;
	
}