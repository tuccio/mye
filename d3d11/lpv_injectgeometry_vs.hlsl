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

	uint i = input.vertexID / g_lpv.rsmResolution;
	uint j = input.vertexID % g_lpv.rsmResolution;

	int3 texcoords = int3(i, j, 0);

	float3 position = g_position.Load(texcoords).xyz;
	float3 normal   = g_normal.Load(texcoords).xyz;
	float4 viewZ    = abs(mul(g_camera.view, float4(position, 1)).z);

	// Geometry volume is shifted by half a cell as in crytek paper, so that
	// the edges of LPV cell are placed in the center of the geometry volume,
	// achieving better interpolation during the propagation

	//float3 cell = (position - g_lpv.minCorner + g_lpv.geometryInjectionBias * normal) / g_lpv.cellSize - .5f;
	//float3 cell = (position - g_lpv.minCorner) / g_lpv.cellSize + .5f + g_lpv.geometryInjectionBias * normal;
	float3 cell = (position - g_lpv.minCorner) / g_lpv.cellSize - .5f - g_lpv.geometryInjectionBias * normal;

	VSOutput output;

	output.normal = normal;
	output.cell   = int3(cell);

	float3 cellCS     = 2.f * cell / g_lpv.lpvResolution - 1.f;
	output.positionCS = float4(cellCS.x, - cellCS.y, cellCS.z, 1.f);

	output.surfelArea = 4.f * tan(g_camera.fovy * .5f) / g_camera.ratio;

	return output;

}