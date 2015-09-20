#include "camera_transform.hlsli"
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
	float  surfelArea : SURFELAREA;
};

VSOutput main(VSInput input)
{

	uint i = input.vertexID / g_lpv.rsmSamples;
	uint j = input.vertexID % g_lpv.rsmSamples;

	int3 texcoords = int3(i, j, 0);

	float4 hPos     = g_position.Load(texcoords);
	float3 position = hPos.xyz;
	float3 normal   = g_normal.Load(texcoords).xyz;

	// Geometry volume is shifted by half a cell as in crytek paper, so that
	// the edges of LPV cell are placed in the center of the geometry volume,
	// achieving better interpolation during the propagation

	float3 cell = (position - g_lpv.minCorner) / g_lpv.cellSize /*- g_lpv.geometryInjectionBias * normal*/ - .5f;

	VSOutput output;

	output.normal = normal;
	output.cell   = int3(cell);

	float3 cellCS = (output.cell + .5f) / g_lpv.lpvResolution;
	cellCS.xy     = 2.f * cellCS.xy - 1.f;

	output.positionCS = float4(cellCS.x, - cellCS.y, cellCS.z, 1.f);

	output.surfelArea = hPos.w;

	return output;

}