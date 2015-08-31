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
	float3 normal   = g_normal.Load(texcoords).xyz;
	float3 flux     = g_flux.Load(texcoords).rgb;

	// If a VPL direction doesn't point towards the center of the cell, it
	// should not contribute to the cell flux, but rather be injected
	// to the cell it is pointing to

	float3 cell = (position - g_lpv.minCorner) / g_lpv.cellSize + g_lpv.fluxInjectionBias * normal;

	VSOutput output;

	output.normal = normal;
	output.flux   = flux;
	output.cell   = int3(cell);

	float3 cellCS     = 2.f * cell / g_lpv.lpvResolution - 1.f;
	output.positionCS = float4(cellCS.x, - cellCS.y, cellCS.z, 1.f);

	return output;
	
}