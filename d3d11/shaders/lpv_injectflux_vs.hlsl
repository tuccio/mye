#include "register_slots.hlsli"
#include "lpv.hlsli"
#include "renderer_configuration.hlsli"
#include "common_samplers.hlsli"

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

	uint i = input.vertexID / g_lpv.rsmSamples;
	uint j = input.vertexID % g_lpv.rsmSamples;

	float2 texcoords = (float2(i, j) + .5f) / g_lpv.rsmSamples;

	RSMTexel texel = RSMRead(texcoords);

	// If a VPL direction doesn't point towards the center of the cell, it
	// should not contribute to the cell flux, but rather be injected
	// to the cell it is pointing to

	float3 cell = (texel.position - g_lpv.minCorner) / g_lpv.cellSize + g_lpv.fluxInjectionBias * texel.normal + .5f;
	VSOutput output;

	output.normal = texel.normal;
	output.flux   = texel.flux /** (1024 / g_lpv.rsmSamples)*/;
	output.cell   = int3(cell);

	float3 cellCS     = 2.f * cell / g_lpv.lpvResolution - 1.f;
	output.positionCS = float4(cellCS.x, - cellCS.y, cellCS.z, 1.f);

	return output;
	
}