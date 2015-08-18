#include "register_slots.hlsli"
#include "lpv.hlsli"
#include "renderer_configuration.hlsli"

#include "rsm_read.hlsli"


struct VSInput
{
	uint vertexID : SV_VertexID;
};

struct VSOutput
{
	float4 positionCS : SV_Position;
	float3 cell       : CELL;
	float3 normal     : NORMAL;
	float3 flux       : FLUX;
};

VSOutput main(VSInput input)
{

	uint i = input.vertexID / g_lpv.rsmResolution;
	uint j = input.vertexID % g_lpv.rsmResolution;

	int3 texcoords = int3(i, j, 0);

	float3 position = g_position.Load(texcoords).xyz;

	VSOutput output;

	output.normal     = g_normal.Load(texcoords).xyz;
	output.flux       = g_flux.Load(texcoords).rgb;

	// If a VPL direction doesn't point towards the center of the cell, it
	// should not contribute to the cell flux, but rather be injected
	// to the cell it is pointing to

	//output.cell       = LPVGetGridCell(position) + .5f * output.normal;
	output.cell = (position - g_lpv.minCorner) / g_lpv.cellSize + .5f * output.normal;

	float invLPVResolution = 1.f / g_lpv.lpvResolution;

	output.positionCS = float4(2.f * output.cell.x * invLPVResolution - 1.f,
							   1.f - 2.f * output.cell.y * invLPVResolution,
							   0.f,
	                           1.f);

	return output;
	
}